#include <lkeyparser/parser.hpp>

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
        cout << name << "\n";

        Parser terminal;
        terminal.addKey(Key('a', "a"), 0, 2);
        terminal.addKey(Key('b', "b"), 0);
        terminal.addKey(Key('c', "c"), 2, 4);
        terminal.addKey(Key('d', "d"));

        Args input_1 = {"qq", "-a", "1", "2", "-b", "-c", "1", "2", "3", "-d", "1", "2", "3", "4", "5"};
        Args input_2 = {"qq", "-a", "1", "2", "3", "4", "-b", "1", "2", "-c", "-d", "1", "2", "3", "4"};
        Args input_3 = {"qq", "-d", "1", "-a", "-b", "1", "-c", "1", "2", "3", "4", "5", "6", "7", "8"};
        Args input_4 = {"qq", "-d", "1", "2", "3", "-c", "1", "2", "-b", "-a", "1", "2", "3", "4", "5"};
        Args input_5 = {"qq", "w", "-a", "1", "-b", "-c", "1", "2", "3", "4", "5", "-d", "1", "2", "3"};

        cout << offset << "a: 0-2,  b: 0,  c: 2-4,  d: all\n";
        cout << offset << "Terminal input 1: " << input_1 << "\n" << terminal.parse(input_1) << "\n";
        cout << offset << "Terminal input 2: " << input_2 << "\n" << terminal.parse(input_2) << "\n";
        cout << offset << "Terminal input 3: " << input_3 << "\n" << terminal.parse(input_3) << "\n";
        cout << offset << "Terminal input 4: " << input_4 << "\n" << terminal.parse(input_4) << "\n";
        cout << offset << "Terminal input 5: " << input_5 << "\n" << terminal.parse(input_5) << "\n";
    }
};

int main() {
    TestKey::parser_test_1("parser 1:", "");
    return 0;
}