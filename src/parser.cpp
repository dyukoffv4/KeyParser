#include "parser.hpp"

keyparser::Parser::Parser() {
    keys.push_back({Key::getRoot(), nullptr});
}

keyparser::Parser &keyparser::Parser::operator=(const Parser &parser) {
    keys.clear();
    for (auto &i : parser.keys) keys.push_back(i);
    return *this;
}

template<class T>
void keyparser::Parser::addKey(const T& data, Parser* parser) {
    delKey(data);
    keys.push_back({Key(data), parser});
}

template<class T>
void keyparser::Parser::delKey(const T& data) {
    for (auto i = keys.begin(); i != keys.end(); i++) {
        if (i->first == Key(data)) {
            keys.erase(i);
            break;
        }
    }
}

template<class T>
std::pair<keyparser::Key, keyparser::Parser*>& keyparser::Parser::getKey(const T& data) {
    for (auto& i : keys) if (i.first == Key(data)) return i;
    throw std::invalid_argument("# Parser.getKey: Key \"" + Key(data).fname() + "\" doesn't exist!");
}

void keyparser::Parser::setupKeyParser(const Key& key, Parser* parser) {
    for (auto& i : keys) {
        if (i.first == key) {
            i.second = parser;
            break;
        }
    }
}

void keyparser::Parser::setupKeyArgnum(const Key& key, int f_num, int s_num = -1) {
    for (auto& i : keys) {
        if (i.first == key) {
            i.first.setRange(f_num, s_num);
            break;
        }
    }
}

keyparser::Tasks keyparser::Parser::parse(int argc, char* argv[]) {
    Args data;
	for (int i = 1; i < argc; i++) data.push_back(argv[i]);
    return parse(data);
}

keyparser::Tasks keyparser::Parser::parse(Args input) {
    Tasks tasks(keys.front().first);

    for (auto &i : input) {
        try {
            int ca = checkArgument(i);
            if (ca == argType::ARG) {
                if (!tasks.pushArg(i)) break;               // Log about end of root range!
                continue;
            }
            if (ca == argType::AKEY) {
                if (!tasks.pushArg(i.substr(1))) break;     // Log about end of root range!
                continue;
            }
            if (ca == argType::SKEY) {
                // check previous task if it hasn't self parser and hasn't enough arguments
                tasks.pushKey(getKey(i[1]).first);
                continue;
            }
            if (ca == argType::LKEY) {
                // check previous task if it hasn't self parser and hasn't enough arguments
                tasks.pushKey(getKey(i.substr(2)).first);
                continue;
            }
        }
        catch (std::invalid_argument e) {
            std::cout << e.what() << '\n';
        }
    }

    for (auto &i : tasks.data) {
        // parse all level of keys // back-tracking
    }

    return tasks;
}


// for (auto &i : input) {
//     int arg_type = checkArgument(i);

//     if (arg_type < 2) {
//         if (tasks.back().first[tasks.back().second.size() + 1] == Key::HG) tasks.push_back({keys.front(), Args()});
//         if (arg_type == 0) tasks.back().second.push_back(i);
//         else tasks.back().second.push_back(i.substr(1));
//     }

//     else if (arg_type < 4) {
//         if (tasks.back().first[tasks.back().second.size()] == Key::LW) {
//             std::cout << "# Parser.parser: Invalid number of arguments for key \"" << tasks.back().first.fname() << "\"!\n";
//             tasks.erase(--tasks.end());
//         }
//         if (arg_type == 2) tasks.push_back({getKey(i[1]), Args()});
//         else tasks.push_back({getKey(i.substr(2)), Args()});
//     }
// }