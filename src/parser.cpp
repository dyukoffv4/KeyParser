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

void keyparser::Parser::addKey(const Key& key, int f_num, int s_num, Parser* parser) {
    if (f_num > s_num && s_num > -1) throw std::invalid_argument("# Parser.addKey: First num can't be bigger then second!");
    delKey(key);
    if (key.full()) {
        parsers[Key(key.sname())] = parser;
        parsers[Key(key.lname())] = parser;
        ranges[Key(key.sname())] = {f_num, s_num};
        ranges[Key(key.lname())] = {f_num, s_num};
        fullkeys.insert({Key(key.sname()), Key(key.lname())});
        fullkeys.insert({Key(key.lname()), Key(key.sname())});
    }
    else {
        parsers[key] = parser;
        ranges[key] = {f_num, s_num};
    }
}

void keyparser::Parser::addKey(const Key& key, int f_num, Parser* parser) {
    addKey(key, f_num, f_num, parser);
}

void keyparser::Parser::addKey(const Key& key, Parser* parser) {
    addKey(key, -1, -1, parser);
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

void keyparser::Parser::setKeyParser(const Key& key, Parser* parser) {
    if (key.full()) {
        if (parsers.count(Key(key.lname())) && parsers.count(Key(key.sname()))) {
            parsers[Key(key.sname())] = parser;
            parsers[Key(key.lname())] = parser;
        }
        else std::cout << "# Parser.parser: Key \"" << key.fname() << "\" doesn't exist!\n";
    }
    else if (parsers.count(key)) {
        parsers[key] = parser;
        if (fullkeys.count(key)) parsers[fullkeys.at(key)] = parser;
    }
    else std::cout << "# Parser.parser: Key \"" << key.fname() << "\" doesn't exist!\n";
}

void keyparser::Parser::setKeyArgnum(const Key& key, int f_num, int s_num) {
    if (f_num > s_num && s_num > -1) throw std::invalid_argument("# Parser.setKeyArgnum: First num can't be bigger then second!");
    if (key.full()) {
        if (ranges.count(Key(key.lname())) && ranges.count(Key(key.sname()))) {
            ranges[Key(key.sname())] = {f_num, s_num};
            ranges[Key(key.lname())] = {f_num, s_num};
        }
        else std::cout << "# Parser.parser: Key \"" << key.fname() << "\" doesn't exist!\n";
    }
    else if (ranges.count(key)) {
        ranges[key] = {f_num, s_num};
        if (fullkeys.count(key)) ranges[fullkeys.at(key)] = {f_num, s_num};
    }
    else std::cout << "# Parser.parser: Key \"" << key.fname() << "\" doesn't exist!\n";
}

void keyparser::Parser::setKeyArgnum(const Key& key, int f_num) {
    setKeyArgnum(key, f_num, f_num);
}

void keyparser::Parser::setArgnum(int f_num, int s_num) {
    if (f_num > s_num && s_num > -1) throw std::invalid_argument("# Parser.setKeyArgnum: First num can't be bigger then second!");
    ranges[Key::getRoot()] = {f_num, s_num};
}

void keyparser::Parser::setArgnum(int f_num) {
    ranges[Key::getRoot()] = {f_num, f_num};
}

keyparser::Tasks keyparser::Parser::parse(int argc, char* argv[]) {
    Args data;
	for (int i = 1; i < argc; i++) data.push_back(argv[i]);
    return parse(data);
}

keyparser::Tasks keyparser::Parser::parse(Args input) {
    Tasks tasks;
    Key curr_key = Key::getRoot();
    unsigned curr_argnum = 0;

    for (auto &i : input) {
        int ca = checkArgument(i);
        if (ca < 2) {
            if (checkZoneStat(curr_argnum + 1, ranges[curr_key]) == zoneType::HG) {
                tasks.pushKey(curr_key = Key::getRoot());
                curr_argnum = tasks.root.size();
                if (checkZoneStat(curr_argnum + 1, ranges[curr_key]) == zoneType::HG) {
                    throw std::invalid_argument("# Parser.parser: Too much arguments for key \"" + curr_key.fname() + "\"!\n");
                }
            }
            if (ca == argType::ARG) tasks.pushArg(i);
            else tasks.pushArg(i.substr(1));
            curr_argnum++;
        }
        else {
            Key next_key = ca == argType::SKEY ? Key(i[1]) : Key(i.substr(2));
            if (parsers.count(next_key)) {
                tasks.pushKey(curr_key = next_key);
                curr_argnum = 0;
            }
            else throw std::invalid_argument("# Parser.parser: Key \"" + next_key.fname() + "\" doesh't exist!\n");
        }
    }

    if (checkZoneStat(tasks.root.size(), ranges[Key::getRoot()]) == zoneType::LW) {
        throw std::invalid_argument("# Parser.parser: Not enough parametrs for root key!\n");
    }

    for (auto &i : tasks.keys) {
        Parser* parser = parsers[i.first];
        if (parser) i.second = parser->parse(i.second.root); 
        else if (i.first != Key::getRoot() && checkZoneStat(i.second.root.size(), ranges[i.first]) == zoneType::LW) {
            throw std::invalid_argument("# Parser.parser: Not enough parametrs for key \"" + curr_key.fname() + "\"!\n");
        }
    }

    return tasks;
}

keyparser::Tasks keyparser::Parser::softParse(int argc, char* argv[]) {
    Args data;
	for (int i = 1; i < argc; i++) data.push_back(argv[i]);
    return softParse(data);
}

keyparser::Tasks keyparser::Parser::softParse(Args input) {
    Tasks tasks;
    Key curr_key = Key::getRoot();
    unsigned curr_argnum = 0;

    for (auto &i : input) {
        try {
            int ca = checkArgument(i);
            if (ca < 2) {
                if (checkZoneStat(curr_argnum + 1, ranges[curr_key]) == zoneType::HG) {
                    curr_key = Key::getRoot();
                    curr_argnum = tasks.root.size();
                    if (checkZoneStat(curr_argnum + 1, ranges[curr_key]) == zoneType::HG) continue;
                    tasks.pushKey(curr_key);
                }
                if (ca == argType::ARG) tasks.pushArg(i);
                else tasks.pushArg(i.substr(1));
                curr_argnum++;
            }
            else if (parsers.count(ca == argType::SKEY ? Key(i[1]) : Key(i.substr(2)))) {
                tasks.pushKey(curr_key = ca == argType::SKEY ? Key(i[1]) : Key(i.substr(2)));
                curr_argnum = 0;
            }
        }
        catch (std::invalid_argument e) {
            std::cout << "# Parser.parser->\n" << e.what() << "\n";
        }
    }

    if (checkZoneStat(tasks.root.size(), ranges[Key::getRoot()]) == zoneType::LW) return Tasks();

    for (auto &i : tasks.keys) {
        Parser* parser = parsers[i.first];
        if (parser) i.second = parser->softParse(i.second.root);
        else if (i.first != Key::getRoot() && checkZoneStat(i.second.root.size(), ranges[i.first]) == zoneType::LW) {
            std::cout << "# Parser.parser: Not enough parametrs for key \"" << curr_key.fname() << "\"!\n";
        }
    }

    return tasks;
}

int keyparser::Parser::checkArgument(const std::string& arg) {
    if (arg.empty()) throw std::invalid_argument("# Parser.checkArgument: Empty argument were got!\n");
    if (arg[0] == '-') {
        if (arg.size() == 2) {
            if (arg[1] != '-') return argType::SKEY;
            throw std::invalid_argument("# Parser.checkArgument: Key expected after \"--\"!\n");
        }
        if (arg.size() > 2) {
            if (arg[1] == '-') return (arg.size() == 3 || arg[2] == '-') ? argType::AKEY : argType::LKEY;
            throw std::invalid_argument("# Parser.checkArgument: Short key expected after \"-\"!\n");
        }
        throw std::invalid_argument("# Parser.checkArgument: Key expected after \"-\"!\n");
    }
    return argType::ARG;
}

int keyparser::Parser::checkZoneStat(unsigned number, std::pair<int, int> zone) {
    if (zone.second < 0) return (zone.first < 0 || zone.first <= number) ? zoneType::IN : zoneType::LW;
    if (zone.first < 0) return (zone.second < 0 || zone.second >= number) ? zoneType::IN : zoneType::HG;
    if (zone.first <= number && zone.second >= number) return zoneType::IN;
    return zone.first > number ? zoneType::LW : zoneType::HG;
}