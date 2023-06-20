#include "../../src/parser.hpp"

using namespace keyparser;
using namespace std;

class TestKey {
public:
    static void range_test(string name, string offset) {
        cout << name << "\n";
        Key key('c', "c");
        cout << offset << "1: " << (key[0] == Key::IN && key[3] == Key::IN && key[5] == Key::IN) << "\n";
        key = Key('c', "c", 3, 3);
        cout << offset << "2: " << (key[0] == Key::LW && key[3] == Key::IN && key[5] == Key::HG) << "\n";
        key = Key('c', "c", 3);
        cout << offset << "3: " << (key[0] == Key::LW && key[3] == Key::IN && key[5] == Key::IN) << "\n";
        key = Key('c', "c", -1, 3);
        cout << offset << "4: " << (key[0] == Key::IN && key[3] == Key::IN && key[5] == Key::HG) << "\n";
        key = Key('c', "c", 3, 5);
        cout << offset << "5: " << (key[0] == Key::LW && key[3] == Key::IN && key[5] == Key::IN && key[7] == Key::HG) << "\n\n";
    }
};

std::ostream& operator<<(std::ostream& out, const keyparser::Args& data) {
    if (data.size() == 0) return out;
    for (auto i = data.begin(); i != --data.end(); i++) out << *i << ", ";
    out << *(--data.end());
    return out;
}

std::ostream& operator<<(std::ostream& out, const keyparser::Tasks& data) {
    for (auto& i : data) out << i.first.fname() << "\t:  " << i.second << '\n';
    return out;
}

int main() {
    // TestKey::range_test("Key 1:", "  ");

    Parser terminal;
    terminal.addKey('a', "a", 0, 2);
    terminal.addKey('b', "b", 0, 0);
    terminal.addKey('c', "c", 2, 4);
    terminal.addKey('d', "d");

    Args input_1 = {"qq", "-a", "1", "2", "-b", "-c", "1", "2", "3", "-d", "1", "2", "3", "4", "5"};
    Args input_2 = {"qq", "-a", "1", "2", "3", "4", "-b", "1", "2", "-c", "-d", "1", "2", "3", "4"};
    Args input_3 = {"qq", "-d", "1", "-a", "-b", "1", "-c", "1", "2", "3", "4", "5", "6", "7", "8"};
    Args input_4 = {"qq", "-d", "1", "2", "3", "-c", "1", "2", "-b", "-a", "1", "2", "3", "4", "5"};
    Args input_5 = {"qq", "w", "-a", "1", "-b", "-c", "1", "2", "3", "4", "5", "-d", "1", "2", "3"};

    cout << "a: 0-2,  b: 0,  c: 2-4,  d: all\n";
    cout << "Terminal input 1: " << input_1 << "\n" << terminal.parse(input_1) << "\n";
    cout << "Terminal input 2: " << input_2 << "\n" << terminal.parse(input_2) << "\n";
    cout << "Terminal input 3: " << input_3 << "\n" << terminal.parse(input_3) << "\n";
    cout << "Terminal input 4: " << input_4 << "\n" << terminal.parse(input_4) << "\n";
    cout << "Terminal input 5: " << input_5 << "\n" << terminal.parse(input_5) << "\n";
}