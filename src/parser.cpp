#include "parser.hpp"

keyparser::Parser::Parser(int f_num, int s_num) {
    if (f_num > s_num && s_num > -1) throw std::invalid_argument("# Parser.Parser: First num can't be bigger then second!");
    parsers[Key::getRoot()] = nullptr;
    ranges[Key::getRoot()] = {f_num, s_num};
}

keyparser::Parser::Parser(int f_num) : Parser(f_num, f_num) {}

keyparser::Parser &keyparser::Parser::operator=(const Parser &parser) {
    parsers.clear();
    ranges.clear();
    fullkeys.clear();
    for (auto &i : parser.parsers) parsers[i.first] = i.second;
    for (auto &i : parser.ranges) ranges[i.first] = i.second;
    for (auto &i : parser.fullkeys) fullkeys.insert({i.first, i.second});
    return *this;
}

void keyparser::Parser::addKey(const Key& key, int f_num, int s_num) {
    if (f_num > s_num && s_num > -1) throw std::invalid_argument("# Parser.addKey: First num can't be bigger then second!");
    delKey(key);
    if (key.full()) {
        parsers[Key(key.sname())] = nullptr;
        parsers[Key(key.lname())] = nullptr;
        ranges[Key(key.sname())] = {f_num, s_num};
        ranges[Key(key.lname())] = {f_num, s_num};
        fullkeys.insert({Key(key.sname()), Key(key.lname())});
        fullkeys.insert({Key(key.lname()), Key(key.sname())});
    }
    else {
        parsers[key] = nullptr;
        ranges[key] = {f_num, s_num};
    }
}

void keyparser::Parser::addKey(const Key& key, int f_num) {
    addKey(key, f_num, f_num);
}

void keyparser::Parser::addKey(const Key& key, Parser* parser) {
    delKey(key);
    if (key.full()) {
        parsers[Key(key.sname())] = parser;
        parsers[Key(key.lname())] = parser;
        ranges[Key(key.sname())] = {-1, -1};
        ranges[Key(key.lname())] = {-1, -1};
        fullkeys.insert({Key(key.sname()), Key(key.lname())});
        fullkeys.insert({Key(key.lname()), Key(key.sname())});
    }
    else {
        parsers[key] = parser;
        ranges[key] = {-1, -1};
    }
}

void keyparser::Parser::delKey(const Key& key) {
    if (key.full()) {
        if (parsers.count(Key(key.lname())) && parsers.count(Key(key.sname()))) {
            parsers.erase(parsers.find(Key(key.lname())));
            parsers.erase(parsers.find(Key(key.sname())));
            ranges.erase(ranges.find(Key(key.lname())));
            ranges.erase(ranges.find(Key(key.sname())));
            fullkeys.erase(fullkeys.find(Key(key.lname())));
            fullkeys.erase(fullkeys.find(Key(key.sname())));
        }
    }
    else if (parsers.count(key)) {
        parsers.erase(parsers.find(key));
        ranges.erase(ranges.find(key));
    }
}

void keyparser::Parser::setArgnum(int f_num, int s_num) {
    if (f_num > s_num && s_num > -1) throw std::invalid_argument("# Parser.setArgnum: First num can't be bigger then second!");
    ranges[Key::getRoot()] = {f_num, s_num};
}

void keyparser::Parser::setArgnum(int f_num) {
    ranges[Key::getRoot()] = {f_num, f_num};
}

keyparser::Task keyparser::Parser::parse(int argc, char* argv[]) {
    Args input;
	for (int i = 1; i < argc; i++) input.push_back(argv[i]);

    std::pair<Task, int> result = hardParse(input, 0, 0, Key::getRoot());
    if (result.second == input.size()) return result.first;
    throw std::invalid_argument("# Parser.parse: Too much arguments were entered!\n");
}

keyparser::Task keyparser::Parser::parse(Args input) {
    std::pair<Task, int> result = hardParse(input, 0, 0, Key::getRoot());
    if (result.second == input.size()) return result.first;
    throw std::invalid_argument("# Parser.parse: Too much arguments were entered!\n");
}

std::pair<keyparser::Task, int> keyparser::Parser::hardParse(const Args& input, int curr, int level, const Key& this_key) {
    Task tasks;
    Task* push_task = &tasks;
    Key push_key = Key::getRoot(), root_key = Key::getRoot(), next_key = Key::getRoot();

    for (int i = curr; i < input.size(); i++) {
        std::pair<int, int> argt = checkArg(input[i]);

        if (argt.second == argType::ARG) {
            if (checkZone(push_task->argnum() + 1, ranges[push_key]) == zoneType::HG) {
                if (push_key == root_key) return {tasks, i};
                push_key = root_key;
                push_task = &tasks;
                if (checkZone(push_task->argnum() + 1, ranges[push_key]) == zoneType::HG) return {tasks, i};
            }
            push_task->addArg(input[i]);
            continue;
        }
        
        if (argt.first - level == 1) {
            next_key = argt.second == argType::SKEY ? Key(input[i][argt.first]): Key(input[i].substr(argt.first + 1));
            if (!parsers.count(next_key)) {
                throw std::invalid_argument("# Parser.parse: Key \"" + next_key.fname() + "\" doesn't exist!\n");
            }
            tasks.addKey(next_key);
            if (!parsers[next_key]) {
                if (checkZone(push_task->argnum(), ranges[push_key]) == zoneType::LW) {
                    std::string estart = "# Parser.parse: Too low arguments for key \"";
                    if (push_key == root_key) throw std::invalid_argument(estart + this_key.fname() + "\"!\n");
                    throw std::invalid_argument(estart + this_key.fname() + "\"->\"" + push_key.fname() + "\"!\n");
                }
                push_key = next_key;
                push_task = &tasks.childs.back().second;
            }
            else {
                try {
                    std::pair<Task, int> result = parsers[next_key]->hardParse(input, i + 1, level + 1, next_key);
                    tasks.childs.back().second = result.first;
                    i = result.second - 1;
                }
                catch (std::invalid_argument e) {
                    throw std::invalid_argument("# Parser.parse: At key \"" + this_key.fname() + "\":\n" + e.what());
                }
            }
            continue;
        }

        if (argt.first - level < 1) {
            if (checkZone(push_task->argnum(), ranges[push_key]) == zoneType::LW) {
                std::string estart = "# Parser.parse: Too low arguments for key \"";
                if (push_key == root_key) throw std::invalid_argument(estart + this_key.fname() + "\"!\n");
                throw std::invalid_argument(estart + this_key.fname() + "\"->\"" + push_key.fname() + "\"!\n");
            }
            return {tasks, i};
        }
    }

    if (checkZone(push_task->argnum(), ranges[push_key]) == zoneType::LW) {
        std::string estart = "# Parser.parse: Too low arguments for key \"";
        if (push_key == root_key) throw std::invalid_argument(estart + this_key.fname() + "\"!\n");
        throw std::invalid_argument(estart + this_key.fname() + "\"->\"" + push_key.fname() + "\"!\n");
    }

    return {tasks, input.size()};
}

std::pair<int, int> keyparser::Parser::checkArg(const std::string& arg) {
    if (arg.empty()) throw std::invalid_argument("# Parser.checkArg: Empty argument were got!\n");
    
    for (int i = 0; i < arg.size(); i++) {
        if (arg[i] != '-') {
            if (i == 0) return {0, argType::ARG};
            if (arg.size() == i + 1) return {i, argType::SKEY};
            if (arg.size() > i + 1) return {i - 1, argType::LKEY};
            throw std::invalid_argument("# Parser.checkArg: Invalid key format!\n");
        }
    }
    throw std::invalid_argument("# Parser.checkArg: Invalid key format!\n");
}

int keyparser::Parser::checkZone(unsigned number, std::pair<int, int> zone) {
    if (zone.second < 0) return (zone.first < 0 || zone.first <= number) ? zoneType::IN : zoneType::LW;
    if (zone.first < 0) return (zone.second < 0 || zone.second >= number) ? zoneType::IN : zoneType::HG;
    if (zone.first <= number && zone.second >= number) return zoneType::IN;
    return zone.first > number ? zoneType::LW : zoneType::HG;
}