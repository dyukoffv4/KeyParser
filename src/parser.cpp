#include "parser.hpp"

keyparser::Parser::Parser(int f_num, int s_num) {
    if (f_num > s_num && s_num > -1) throw std::invalid_argument("# Parser.Parser: First num can't be bigger then second!");
    parsers[Key::getRoot()] = nullptr;
    ranges[Key::getRoot()] = {f_num, s_num};
}

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
        else std::cout << "# Parser.parser: Key \"" << key.fname() << "\" doesh't exist!\n\n";
    }
    else if (parsers.count(key)) {
        parsers[key] = parser;
        if (fullkeys.count(key)) parsers[fullkeys.at(key)] = parser;
    }
    else std::cout << "# Parser.parser: Key \"" << key.fname() << "\" doesh't exist!\n\n";
}

void keyparser::Parser::setKeyArgnum(const Key& key, int f_num, int s_num) {
    if (f_num > s_num && s_num > -1) throw std::invalid_argument("# Parser.setKeyArgnum: First num can't be bigger then second!");
    if (key.full()) {
        if (ranges.count(Key(key.lname())) && ranges.count(Key(key.sname()))) {
            ranges[Key(key.sname())] = {f_num, s_num};
            ranges[Key(key.lname())] = {f_num, s_num};
        }
        else std::cout << "# Parser.parser: Key \"" << key.fname() << "\" doesh't exist!\n\n";
    }
    else if (ranges.count(key)) {
        ranges[key] = {f_num, s_num};
        if (fullkeys.count(key)) ranges[fullkeys.at(key)] = {f_num, s_num};
    }
    else std::cout << "# Parser.parser: Key \"" << key.fname() << "\" doesh't exist!\n\n";
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
        try {
            int ca = checkArgument(i);
            if (ca < 2) {
                if (checkZoneStat(curr_argnum + 1, ranges[curr_key]) == zoneType::HG) {
                    if (curr_key == Key::getRoot() || checkZoneStat(tasks.root.size() + 1, ranges[Key::getRoot()]) == zoneType::HG) {
                        std::cout << "# Parser.parser: Too much arguments for key \"" << curr_key.fname() << "\"!\n\n";
                        break;
                    }
                    tasks.pushKey(Key::getRoot());
                    curr_key = Key::getRoot();
                }
                if (ca == argType::ARG) tasks.pushArg(i);
                else tasks.pushArg(i.substr(1));
                curr_argnum++;
                continue;
            }
            if (ca < 4) {
                if (parsers.count(ca == argType::SKEY ? Key(i[1]) : Key(i.substr(2)))) {
                    if (!parsers[curr_key] && checkZoneStat(curr_argnum, ranges[curr_key]) == zoneType::LW) {
                        std::cout << "# Parser.parser: Invalid number of arguments for key \"" << curr_key.fname() << "\"!\n\n";
                        if (!tasks.popKey()) return Tasks();
                    }
                    curr_key = ca == argType::SKEY ? Key(i[1]) : Key(i.substr(2));
                    tasks.pushKey(curr_key);
                    curr_argnum = 0;
                }
                else std::cout << "# Parser.parser: Key \"" << (ca == argType::SKEY ? Key(i[1]) : Key(i.substr(2))).fname() << "\" doesh't exist!\n\n";
                continue;
            }
        }
        catch (std::invalid_argument e) {
            std::cout << "# Parser.parser:\n" << e.what() << "\n\n";
        }
    }

    for (auto &i : tasks.keys) {
        Parser* parser = parsers[i.first];
        if (parser) i.second = parser->parse(i.second.root); 
    }

    return tasks;
}

// 0 - not a key;    1 - future key;    2 - short key;    3 - long key;
int keyparser::Parser::checkArgument(const std::string& arg) {
    if (arg.empty()) throw std::invalid_argument("# Parser.parser: Empty argument were got!\n");
    if (arg[0] == '-') {
        if (arg.size() == 2) {
            if (arg[1] != '-') return 2;
            throw std::invalid_argument("# Parser.parser: Key expected after \"--\"!\n");
        }
        if (arg.size() > 2) {
            if (arg[1] == '-') return (arg[2] == '-') ? 1 : 3;
            throw std::invalid_argument("# Parser.parser: Short key expected after \"-\"!\n");
        }
        throw std::invalid_argument("# Parser.parser: Key expected after \"-\"!\n");
    }
    return 0;
}

// 0 - lower;    1 - in range;    2 - higher;
int keyparser::Parser::checkZoneStat(unsigned number, std::pair<int, int> zone) {
    if (zone.second < 0) return (zone.first < 0 || zone.first <= number) ? 1 : 0;
    if (zone.first < 0) return (zone.second < 0 || zone.second >= number) ? 1 : 2;
    return (zone.first <= number && zone.second >= number) ? 1 : (zone.first > number ? 0 : 2);
}