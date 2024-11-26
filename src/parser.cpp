#include "include/keyparser/parser.hpp"

/// @brief Create Parser with partially limited number of root parameters
/// @param f_num Lower limit of parameters [int]
/// @param s_num Upper limit of parameters [int]
keyparser::Parser::Parser(unsigned f_num, unsigned s_num) {
    if (f_num > s_num) throw std::invalid_argument("# Parser.Parser: First num can't be bigger then second!");
    range_0 = f_num;
    range_1 = s_num;
}

/// @brief Create Parser with fixed number of root parameters
/// @param f_num Number of parameters [int]
keyparser::Parser::Parser(unsigned num) {
    range_0 = num;
    range_1 = num;
}

keyparser::Parser::Parser() {
    range_0 = 0;
    range_1 = uint_max;
}

keyparser::Parser &keyparser::Parser::operator=(const Parser &parser) {
    parsers.clear();
    for (auto &i : parser.parsers) parsers[i.first] = i.second;
    range_0 = parser.range_0;
    range_1 = parser.range_1;
    return *this;
}

/// @brief Add key with partially limited number of parameters
/// @param key Key [Key&]
/// @param f_num Lower limit of parameters [int]
/// @param s_num Upper limit of parameters [int]
keyparser::Parser* keyparser::Parser::addKey(const Key& key, unsigned f_num, unsigned s_num, bool force) {
    if (f_num > s_num) throw std::invalid_argument("# Parser.addKey: First num can't be bigger then second!");
    if (force) delKey(key, false);
    else if (getKey(key, false)) return nullptr;
    return &(parsers[key] = Parser(f_num, s_num));
}

/// @brief Add key with fixed number of parameters
/// @param key Key [Key&]
/// @param f_num Number of parameters [int]
keyparser::Parser* keyparser::Parser::addKey(const Key& key, unsigned num, bool force) {
    if (force) delKey(key, false);
    else if (getKey(key, false)) return nullptr;
    return &(parsers[key] = Parser(num));
}

keyparser::Parser* keyparser::Parser::addKey(const Key& key, bool force) {
    if (force) delKey(key, false);
    else if (getKey(key, false)) return nullptr;
    return &(parsers[key] = Parser());
}

keyparser::Parser* keyparser::Parser::getKey(const Key& key, bool exact) {
    if (exact) return parsers.count(key) ? &parsers[key] : nullptr;
    for (auto i = parsers.begin(); i != parsers.end(); i++) {
        if (i->first ^= key) return &(i->second);
    }
    return nullptr;
}

/// @brief Delete a full key from storage
/// @param key Variant of a key to delete [Key&]
void keyparser::Parser::delKey(const Key& key, bool exact) {
    if (exact) {
        if (parsers.count(key)) parsers.erase(key);
    }
    else {
        for (auto i = parsers.begin(); i != parsers.end();) {
            if (i->first ^= key) i = parsers.erase(i);
            else i++;
        }
    }
}

/// @brief Set limited number of root parameters
/// @param f_num Lower limit of parameters [int]
/// @param s_num Upper limit of parameters [int]
void keyparser::Parser::setRange(unsigned f_num, unsigned s_num) {
    if (f_num > s_num && s_num > -1) throw std::invalid_argument("# Parser.setRange: First num can't be bigger then second!");
    range_0 = f_num;
    range_1 = s_num;
}

/// @brief Set fixed of root parameters
/// @param f_num Number of parameters [int]
void keyparser::Parser::setRange(unsigned f_num) {
    range_0 = f_num;
    range_1 = f_num;
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
keyparser::Task keyparser::Parser::parse(const Args& input) {
    Task result = dumbParse(input);
    try { upgradeTasks(result); }
    catch (trace_argument_error e) {
        switch (e.label) {
        case 1:
            throw std::invalid_argument("# Parser.parse: Key " + std::string(e.what()) + " doesn't exist!\n");
        case 2:
            throw std::invalid_argument("# Parser.parse: Key " + std::string(e.what()) + " received few parameters!\n");
        }
    }
    if (range_0 > result.argnum()) throw std::invalid_argument("# Parser.parse: Received few parameters!\n");
    if (range_1 < result.argnum()) throw std::invalid_argument("# Parser.parse: Received much parameters!\n");
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
            task_stack.push_back(&task_stack.back()->childs.back());
            continue;
        }
        throw std::invalid_argument("# Parser.parse: Undefined argument \"" + input[i] + "\"!\n");
    }
    return tasks;
}

// private methods

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

void keyparser::Parser::upgradeTasks(Task& task) {
    for (auto &i : task.childs) {
        bool child_not_exist = true;
        for (auto &j : parsers) {
            if (i.name ^= j.first) {
                i.name = j.first;
                try {
                    j.second.upgradeTasks(i);
                }
                catch (trace_argument_error e) {
                    throw trace_argument_error(i.name.fname() + " -> " + e.what(), e.label);
                }
                if (range_0 > i.argnum()) throw trace_argument_error(i.name.fname(), 2);
                if (range_1 < i.argnum()) {
                    task.root.insert(task.root.end(), i.root.begin() + j.second.range_1, i.root.end());
                    i.root.erase(i.root.begin() + j.second.range_1, i.root.end());
                }
                child_not_exist = false;
                break;
            }
        }
        if (child_not_exist) throw trace_argument_error(i.name.fname(), 1);
    }
}