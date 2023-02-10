#include "../../src/terminal.hpp"

using namespace KP;
using namespace std;

class TestKey {
public:
    static void range_test(string name, string offset) {
        cout << name << "\n";
        Key key('c');
        cout << offset << "1: " << (key[0] == Key::ZS_I && key[3] == Key::ZS_I && key[5] == Key::ZS_I) << "\n";
        key = Key('c', 3, 3);
        cout << offset << "2: " << (key[0] == Key::ZS_L && key[3] == Key::ZS_I && key[5] == Key::ZS_H) << "\n";
        key = Key('c', 3);
        cout << offset << "3: " << (key[0] == Key::ZS_L && key[3] == Key::ZS_I && key[5] == Key::ZS_I) << "\n";
        key = Key('c', -1, 3);
        cout << offset << "4: " << (key[0] == Key::ZS_I && key[3] == Key::ZS_I && key[5] == Key::ZS_H) << "\n";
        key = Key('c', 3, 5);
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

int main() {
    TestKey::range_test("Key 1:", "  ");
    TestKey::state_test("Key 2:", "  ");
}