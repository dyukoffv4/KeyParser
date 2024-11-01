#include "parser.hpp"

/// @brief Create Parser with partially limited number of root parameters
/// @param f_num Lower limit of parameters [int]
/// @param s_num Upper limit of parameters [int]
keyparser::Parser::Parser(int f_num, int s_num) {
    if (f_num > s_num && s_num > -1) throw std::invalid_argument("# Parser.Parser: First num can't be bigger then second!");
    parsers[Key::getRoot()] = nullptr;
    ranges[Key::getRoot()] = {f_num, s_num};
}

/// @brief Create Parser with fixed number of root parameters
/// @param f_num Number of parameters [int]
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

/// @brief Add key with partially limited number of parameters
/// @param key Key [Key&]
/// @param f_num Lower limit of parameters [int]
/// @param s_num Upper limit of parameters [int]
void keyparser::Parser::addKey(const Key& key, int f_num, int s_num) {
    if (f_num > s_num && s_num > -1) throw std::invalid_argument("# Parser.addKey: First num can't be bigger then second!");
    delKey(key);
    if (key.full()) {
        parsers[Key(key.sname())] = nullptr;
        parsers[Key(key.lname())] = nullptr;
        ranges[Key(key.sname())] = {f_num, s_num};
        ranges[Key(key.lname())] = {f_num, s_num};
        fullkeys.insert({Key(key.sname()), key});
        fullkeys.insert({Key(key.lname()), key});
    }
    else {
        parsers[key] = nullptr;
        ranges[key] = {f_num, s_num};
        fullkeys.insert({key, key});
    }
}

/// @brief Add key with fixed number of parameters
/// @param key Key [Key&]
/// @param f_num Number of parameters [int]
void keyparser::Parser::addKey(const Key& key, int f_num) {
    addKey(key, f_num, f_num);
}

/// @brief Add key with own subkeys
/// @param key Key [Key&]
/// @param parser Another parser with setted up keys [Parser*]
void keyparser::Parser::addKey(const Key& key, Parser* parser) {
    addKey(key, -1, -1);
    if (key.full()) {
        parsers[Key(key.sname())] = parser;
        parsers[Key(key.lname())] = parser;
    }
    else parsers[key] = parser;
}

/// @brief Delete a full key from storage
/// @param key Variant of a key to delete [Key&]
void keyparser::Parser::delKey(const Key& key) {
    Key fkey = key;
    if (!fkey.full()) {
        if (fullkeys.count(fkey)) fkey = fullkeys.find(fkey)->second;
        else return;
    }

    if (fkey.full()) {
        if (fullkeys.count(Key(fkey.sname())) && fullkeys.count(Key(fkey.lname()))) {
            parsers.erase(parsers.find(Key(fkey.lname())));
            parsers.erase(parsers.find(Key(fkey.sname())));
            ranges.erase(ranges.find(Key(fkey.lname())));
            ranges.erase(ranges.find(Key(fkey.sname())));
            fullkeys.erase(fullkeys.find(Key(fkey.lname())));
            fullkeys.erase(fullkeys.find(Key(fkey.sname())));
        }
    }
    else {
        parsers.erase(parsers.find(fkey));
        ranges.erase(ranges.find(fkey));
        fullkeys.erase(fullkeys.find(fkey));
    }
}

/// @brief Set limited number of root parameters
/// @param f_num Lower limit of parameters [int]
/// @param s_num Upper limit of parameters [int]
void keyparser::Parser::setArgnum(int f_num, int s_num) {
    if (f_num > s_num && s_num > -1) throw std::invalid_argument("# Parser.setArgnum: First num can't be bigger then second!");
    ranges[Key::getRoot()] = {f_num, s_num};
}

/// @brief Set fixed of root parameters
/// @param f_num Number of parameters [int]
void keyparser::Parser::setArgnum(int f_num) {
    ranges[Key::getRoot()] = {f_num, f_num};
}

/// @brief Parses keys and arguments according to the specified settings
/// @param argc Number of arguments [int]
/// @param argv Array of arguments [char**]
/// @return Object with structured keys and parametrs for each key [Task]
keyparser::Task keyparser::Parser::parse(int argc, char* argv[]) {
    Args input;
	for (int i = 1; i < argc; i++) input.push_back(argv[i]);
    return parse(input);
}

/// @brief Parses keys and arguments according to the specified settings
/// @param input List of arguments to parse [vector<string>]
/// @return Object with structured keys and parametrs for each key [Task]
keyparser::Task keyparser::Parser::parse(Args input) {
    Task result = dumbParse(input);
    try { upgradeTasks(result); }
    catch (labeled_error e) {
        switch (e.label) {
        case 1:
            throw std::invalid_argument("# Parser.parse: Key " + std::string(e.what()) + " doesn't exist!\n");
            break;
        case 2:
            throw std::invalid_argument("# Parser.parse: Key " + std::string(e.what()) + " hasn't nested keys!\n");
            break;
        case 3:
            throw std::invalid_argument("# Parser.parse: Key " + std::string(e.what()) + " received few parameters!\n");
            break;
        default:
            throw e;
            break;
        }
    }
    switch (checkZone(result.argnum(), ranges[result.name])) {
        case zoneType::HG:
            throw std::invalid_argument("# Parser.parse: Received much parameters!\n");
            break;
        case zoneType::LW:
            throw std::invalid_argument("# Parser.parse: Received few parameters!\n");
            break;
    }
    return result;
}

/// @brief Parses keys and arguments according to base syntax
/// @param input List of arguments to parse [vector<string>]
/// @return Object with structured keys and parametrs for each key [Task]
keyparser::Task keyparser::Parser::dumbParse(const Args& input) {
    Task tasks;
    std::vector<Task*> task_stack = {&tasks};
    for (int i = 0; i < input.size(); i++) {
        std::pair<int, int> argt = checkArg(input[i]);
        if (argt.second == argType::ARG) {
            task_stack.back()->addArg(input[i]);
            continue;
        }
        if (argt.first - int(task_stack.size()) <= 0) {
            task_stack.erase(task_stack.begin() + argt.first, task_stack.end());
            task_stack.back()->addKey(argt.second == argType::SKEY ? Key(input[i][argt.first]): Key(input[i].substr(argt.first + 1)));
            task_stack.push_back(&task_stack.back()->childs.back().second);
            continue;
        }
        throw std::invalid_argument("# Parser.parse: Undefined argument \"" + input[i] + "\"!\n");
    }
    return tasks;
}

std::pair<int, int> keyparser::Parser::checkArg(const std::string& arg) {
    if (arg.empty()) throw std::invalid_argument("# Parser.checkArg: Empty argument were got!\n");
    
    // Return (0, ...) if Argument, (1+, ...) if Key
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

void keyparser::Parser::upgradeTasks(Task& task) {
    for (auto &i : task.childs) {
        Task& child = i.second;

        if (!parsers.count(child.name)) throw labeled_error(child.name.fname(), 1);
        if (parsers[child.name]) {
            try {
                parsers[child.name]->upgradeTasks(child);
            }
            catch (labeled_error e) {
                throw labeled_error(child.name.fname() + " -> " + e.what(), e.label);
            }
        }
        else if (!child.childs.empty()) throw labeled_error(child.name.fname(), 2);

        switch (checkZone(child.argnum(), ranges[child.name])) {
        case zoneType::HG:
            task.root.assign(child.root.begin() + ranges[child.name].second, child.root.end());
            child.root.erase(child.root.begin() + ranges[child.name].second, child.root.end());
            break;
        case zoneType::LW:
            throw labeled_error(child.name.fname(), 3);
            break;
        }
    }
}