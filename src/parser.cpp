#include "include/keyparser/parser.hpp"

keyparser::Parser::Parser(unsigned f_num, unsigned s_num) {
    if (f_num > s_num) throw std::invalid_argument("# Parser.Parser: First num can't be bigger then second!");
    range_0 = f_num;
    range_1 = s_num;
}

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

keyparser::Parser* keyparser::Parser::addKey(const Key& key, unsigned f_num, unsigned s_num) {
    if (f_num > s_num) throw std::invalid_argument("# Parser.addKey: First num can't be bigger then second!");
    if (getKey(key, false)) return nullptr;
    return &(parsers[key] = Parser(f_num, s_num));
}

keyparser::Parser* keyparser::Parser::addKey(const Key& key, unsigned num) {
    if (getKey(key, false)) return nullptr;
    return &(parsers[key] = Parser(num));
}

keyparser::Parser* keyparser::Parser::addKey(const Key& key) {
    if (getKey(key, false)) return nullptr;
    return &(parsers[key] = Parser());
}

keyparser::Parser* keyparser::Parser::getKey(const Key& key, bool exact) {
    if (exact) return parsers.count(key) ? &parsers[key] : nullptr;
    for (auto i = parsers.begin(); i != parsers.end(); i++) {
        if (i->first ^= key) return &(i->second);
    }
    return nullptr;
}

unsigned keyparser::Parser::delKey(const Key& key, bool exact) {
    if (exact) return parsers.erase(key);
    unsigned erased = 0;
    for (auto i = parsers.begin(); i != parsers.end();) {
        if (i->first ^= key) {
            i = parsers.erase(i);
            erased++;
        }
        else i++;
    }
    return erased;
}

void keyparser::Parser::setRange(unsigned f_num, unsigned s_num) {
    if (f_num > s_num) throw std::invalid_argument("# Parser.setRange: First num can't be bigger then second!");
    range_0 = f_num;
    range_1 = s_num;
}

void keyparser::Parser::setRange(unsigned f_num) {
    range_0 = f_num;
    range_1 = f_num;
}

keyparser::Task keyparser::Parser::parse(int argc, char* argv[]) {
    Args input;
	for (int i = 1; i < argc; i++) input.push_back(argv[i]);
    return parse(input);
}

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

void keyparser::Parser::upgradeTasks(Task& tasks) {
    for (auto &task : tasks.childs) {
        bool child_not_exist = true;
        for (auto &i : parsers) {
            Parser& parser = i.second;
            if (task.name ^= i.first) {
                if (task.keynum()) {
                    try {
                        parser.upgradeTasks(task);
                    }
                    catch (trace_argument_error e) {
                        throw trace_argument_error(task.name.fname() + " -> " + e.what(), e.label);
                    }
                }
                if (parser.range_0 > task.argnum()) throw trace_argument_error(task.name.fname(), 2);
                if (parser.range_1 < task.argnum()) {
                    task.root.insert(task.root.end(), task.root.begin() + parser.range_1, task.root.end());
                    task.root.erase(task.root.begin() + parser.range_1, task.root.end());
                }
                task.name = i.first;
                child_not_exist = false;
                break;
            }
        }
        if (child_not_exist) throw trace_argument_error(task.name.fname(), 1);
    }
}