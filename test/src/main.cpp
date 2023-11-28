#include "../../src/parser.hpp"

using namespace keyparser;
using namespace std;

std::ostream& operator<<(std::ostream& out, const keyparser::Args& data) {
    if (data.size() == 0) return out;
    for (auto i = data.begin(); i != --data.end(); i++) out << *i << ", ";
    out << *(--data.end());
    return out;
}

std::ostream& operator<<(std::ostream& out, const keyparser::Tasks& data) {
    out << data.root << '\n';
    for (auto& i : data.keys) out << i.first.fname() << "\t:  " << i.second << '\n';
    return out;
}

class TestKey {
public:
    static void parser_test_template(string name, string offset, std::vector<Args> inputs) {
        Parser terminal;
        terminal.addKey(Key('a', "a"), 0, 2);
        terminal.addKey(Key('b', "b"), 0);
        terminal.addKey(Key('c', "c"), 2, 4);
        terminal.addKey(Key('d', "d"));

        cout << name << "\n" << offset << "a: 0-2,  b: 0,  c: 2-4,  d: all\n";
        for (auto& i : inputs) {
            cout << offset << "Terminal input 1: " << i << "\n" << terminal.parse(i) << "\n";
        }
    }

    static void parser_test_1(string name, string offset) {
        Args input_1 = {"qq", "-a", "1", "2", "-b", "-c", "1", "2", "3", "-d", "1", "2", "3", "4", "5"};
        Args input_2 = {"qq", "-a", "1", "2", "3", "4", "-b", "1", "2", "-c", "-d", "1", "2", "3", "4"};
        Args input_3 = {"qq", "-d", "1", "-a", "-b", "1", "-c", "1", "2", "3", "4", "5", "6", "7", "8"};
        Args input_4 = {"qq", "-d", "1", "2", "3", "-c", "1", "2", "-b", "-a", "1", "2", "3", "4", "5"};
        Args input_5 = {"qq", "w", "-a", "1", "-b", "-c", "1", "2", "3", "4", "5", "-d", "1", "2", "3"};

        TestKey::parser_test_template(name, offset, {input_1, input_2, input_3, input_4, input_5});
    }

    static void parser_test_2(string name, string offset) {
        Args input_1 = {"qq", "-a", "--s", "2", "-b", "-c", "--a", "2", "3", "-d", "1", "2", "3", "4", "--"};
        Args input_2 = {"qq", "-a", "1", "2", "---", "4", "-b", "1", "2", "-c", "-d", "---", "-c", "3", "4"};

        TestKey::parser_test_template(name, offset, {input_1, input_2});
    }
};

int main() {
    TestKey::parser_test_1("parser 1:", "");
    TestKey::parser_test_2("parser 2:", "");
    return 0;
}