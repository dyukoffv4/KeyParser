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
    static void parser_test_1(string name, string offset) {
        std::vector<Args> input = {
            {"qq", "-a", "1", "2", "-b", "-c", "1", "2", "3", "-d", "1", "2", "3", "4", "5"},
            {"qq", "-a", "1", "2", "3", "4", "-b", "1", "2", "-c", "-d", "1", "2", "3", "4"},
            {"qq", "-d", "1", "-a", "-b", "1", "-c", "1", "2", "3", "4", "5", "6", "7", "8"},
            {"qq", "-d", "1", "2", "3", "-c", "1", "2", "-b", "-a", "1", "2", "3", "4", "5"},
            {"qq", "w", "-a", "1", "-b", "-c", "1", "2", "3", "4", "5", "-d", "1", "2", "3"}
        };

        Parser terminal;
        terminal.addKey(Key('a', "a1"), 0, 2);
        terminal.addKey(Key('b', "b1"), 0);
        terminal.addKey(Key('c', "c1"), 2, 4);
        terminal.addKey(Key('d', "d1"));

        cout << name << "\n" << offset << "a: 0-2,  b: 0,  c: 2-4,  d: all\n";
        for (int i = 0; i < input.size(); i++) {
            cout << offset << "Terminal input " << i + 1 << ": " << input[i] << "\n" << terminal.softParse(input[i]) << "\n";
        }
    }

    static void parser_test_2(string name, string offset) {
        std::vector<Args> input = {
            {"qq", "-b", "--a", "1", "--c", "--b", "2", "-a", "3", "4", "--a", "5"},
            {"qq", "--a", "--b", "1", "---a", "2", "---c", "3", "--b", "4", "---a"}
        };

        Parser terminal, terminal_1;
        terminal_1.addKey(Key('a', "a1"), 0, 2);
        terminal_1.addKey(Key('c', "c1"), 0);
        terminal.addKey(Key('a', "a1"), 0, 2);
        terminal.addKey(Key('b', "b1"), &terminal_1);

        cout << name << "\n" << offset << "a: 0-2,  b: 0, b-a: 0-2, b-c: 0\n";
        for (int i = 0; i < input.size(); i++) {
            cout << offset << "Terminal input " << i + 1 << ": " << input[i] << "\n" << terminal.softParse(input[i]) << "\n";
        }
    }
};

int main() {
    // TestKey::parser_test_1("parser 1:", "");
    TestKey::parser_test_2("parser 2:", "");
    return 0;
}