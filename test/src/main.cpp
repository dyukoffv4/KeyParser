#include "../../src/terminal.hpp"

using namespace KP;
using namespace std;

class TestKey {
public:
    static void range_test(string name, string offset) {
        cout << name << "\n";
        Key key('c', "c");
        cout << offset << "1: " << (key[0] == Key::ZS_I && key[3] == Key::ZS_I && key[5] == Key::ZS_I) << "\n";
        key = Key('c', "c", 3, 3);
        cout << offset << "2: " << (key[0] == Key::ZS_L && key[3] == Key::ZS_I && key[5] == Key::ZS_H) << "\n";
        key = Key('c', "c", 3);
        cout << offset << "3: " << (key[0] == Key::ZS_L && key[3] == Key::ZS_I && key[5] == Key::ZS_I) << "\n";
        key = Key('c', "c", -1, 3);
        cout << offset << "4: " << (key[0] == Key::ZS_I && key[3] == Key::ZS_I && key[5] == Key::ZS_H) << "\n";
        key = Key('c', "c", 3, 5);
        cout << offset << "5: " << (key[0] == Key::ZS_L && key[3] == Key::ZS_I && key[5] == Key::ZS_I && key[7] == Key::ZS_H) << "\n\n";
    }

    static void state_test(string name, string offset) {
        cout << name << "\n";
        Key key('c');
        cout << offset << "1: " << (key.getState() == Key::S) << "\n";
        key = Key('c', "c");
        cout << offset << "2: " << (key.getState() == Key::A) << "\n";
        key = Key("c");
        cout << offset << "3: " << (key.getState() == Key::L) << "\n";
        try {
            key = Key("");
            cout << offset << "4: " << false << "\n\n";
        }
        catch (exception exp) {
            cout << offset << "4: " << true << "\n\n";
        }
    }
};

void test(Args input) {
    cout << "  ";
    for (auto &i : input) cout << i << ' ';
    cout << "\n";
}

int main() {
    TestKey::range_test("Key 1:", "  ");
    TestKey::state_test("Key 2:", "  ");

    Terminal terminal(Terminal::RS_S);
    terminal.setRootRange(3, 3);
    terminal.setRoot(test);
    terminal.setKey(Key('a', "a", 0, 2), test);
    terminal.setKey(Key('b', "b", 0, 0), test);
    terminal.setKey(Key('c', "c", 2, 4), test);
    terminal.setKey(Key('d', "d"), test);

    Args input_1 = {"qq", "-a", "1", "2", "-b", "-c", "1", "2", "3", "-d", "1", "2", "3", "4", "5"};
    Args input_2 = {"qq", "-a", "1", "2", "3", "4", "-b", "1", "2", "-c", "-d", "1", "2", "3", "4"};
    Args input_3 = {"qq", "-d", "1", "-a", "-b", "1", "-c", "1", "2", "3", "4", "5", "6", "7", "8"};
    Args input_4 = {"qq", "-d", "1", "2", "3", "-c", "1", "2", "-b", "-a", "1", "2", "3", "4", "5"};
    Args input_5 = {"qq", "w", "-a", "1", "-b", "-c", "1", "2", "3", "4", "5", "-d", "1", "2", "3"};

    cout << "Terminal input 1 (a b c d):\n"; terminal.execute(input_1); cout << "\n";
    cout << "Terminal input 2 (a b c d):\n"; terminal.execute(input_2); cout << "\n";
    cout << "Terminal input 3 (d a b c):\n"; terminal.execute(input_3); cout << "\n";
    cout << "Terminal input 4 (d c b a):\n"; terminal.execute(input_4); cout << "\n";
    cout << "Terminal input 5 (a b c d):\n"; terminal.execute(input_5); cout << "\n";
}
