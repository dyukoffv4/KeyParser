#include "parser.hpp"

keyparser::Parser::Parser() {
    keys.push_back(Key::getRoot());
}

keyparser::Parser &keyparser::Parser::operator=(const Parser &parser) {
    keys.clear();
    for (auto &i : parser.keys) keys.push_back(i);
    return *this;
}

void keyparser::Parser::addKey(const char& s_data, int f_num, int s_num) {
    delKey(s_data);
    keys.push_back(Key(s_data, f_num, s_num));
}

void keyparser::Parser::addKey(const std::string& l_data, int f_num, int s_num) {
    delKey(l_data);
    keys.push_back(Key(l_data, f_num, s_num));
}

void keyparser::Parser::addKey(const char& s_data, const std::string& l_data, int f_num, int s_num) {
    delKey(s_data);
    delKey(l_data);
    keys.push_back(Key(s_data, l_data, f_num, s_num));
}

template<class T>
void keyparser::Parser::delKey(const T& data) {
    for (auto i = keys.begin(); i != keys.end(); i++) {
        if (*i == Key(data)) {
            keys.erase(i);
            break;
        }
    }
}

template<class T>
keyparser::Key keyparser::Parser::getKey(const T& data) {
    for (auto& i : keys) if (i == Key(data)) return i;
    throw std::invalid_argument("# Parser.getKey: Key \"" + Key(data).fname() + "\" doesn't exist!");
}

void keyparser::Parser::setupRoot(int f_num, int s_num) {
    keys.front() = Key::getRoot(f_num, s_num);
}

keyparser::Tasks keyparser::Parser::parse(int argc, char* argv[]) {
    Args data;
	for (int i = 1; i < argc; i++) data.push_back(argv[i]);
    return parse(data);
}

keyparser::Tasks keyparser::Parser::parse(Args input) {
    auto checkArgument = [](std::string arg) -> int {    // 0 - not key;  1 - nl key;  2 - short key;  3 - long key;  4 - error;
        if (arg.empty()) {
            std::cout << "# Parser.parser: Empty argument were got!\n";
            return 4;
        }
        if (arg[0] == '-') {
            if (arg.size() == 2) {
                if (arg[1] != '-') return 2;
                std::cout << "# Parser.parser: Key expected after \"--\"!\n";
                return 4;
            }
            if (arg.size() > 2) {
                if (arg[1] == '-') return (arg[2] == '-') ? 1 : 3;
                std::cout << "# Parser.parser: Short key expected after \"-\"!\n";
                return 4;
            }
            std::cout << "# Parser.parser: Key expected after \"-\"!\n";
            return 4;
        }
        return 0;
    };

    Tasks tasks = {{keys.front(), Args()}};

    for (auto &i : input) {
        int arg_type = checkArgument(i);

        if (arg_type < 2) {
            if (tasks.back().first[tasks.back().second.size() + 1] == Key::HG) tasks.push_back({keys.front(), Args()});
            if (arg_type == 0) tasks.back().second.push_back(i);
            else tasks.back().second.push_back(i.substr(1));
        }

        else if (arg_type < 4) {
            if (tasks.back().first[tasks.back().second.size()] == Key::LW) {
                std::cout << "# Parser.parser: Invalid number of arguments for key \"" << tasks.back().first.fname() << "\"!\n";
                tasks.erase(--tasks.end());
            }
            if (arg_type == 2) tasks.push_back({getKey(i[1]), Args()});
            else tasks.push_back({getKey(i.substr(2)), Args()});
        }
    }

    return tasks;
}