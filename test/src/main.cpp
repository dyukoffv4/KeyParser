#include "../../src/terminal.hpp"

using namespace KP;

class TestKey {
public:
    static bool diapozon_test_1() {
        Key key('c');
        return key[0] == Key::ZS_I && key[5] == Key::ZS_I && key[-1] == Key::ZS_I;
    }

    static bool diapozon_test_2() {
        Key key('c', 0, 0);
        return key[0] == Key::ZS_I && key[5] == Key::ZS_H && key[-1] == Key::ZS_L;
    }
    
    static bool diapozon_test_3() {
        Key key('c', 3);
        return key[0] == Key::ZS_L && key[5] == Key::ZS_I && key[3] == Key::ZS_I;
    }

    static bool diapozon_test_4() {
        Key key('c', -1, 3);
        return key[0] == Key::ZS_I && key[5] == Key::ZS_H && key[3] == Key::ZS_I;
    }

    static bool diapozon_test_5() {
        Key key('c', 3, 5);
        return key[1] == Key::ZS_L && key[3] == Key::ZS_I && key[5] == Key::ZS_I && key[7] == Key::ZS_H;
    }
};

int main() {
    std::cout << "1: " << TestKey::diapozon_test_1() << std::endl;
    std::cout << "2: " << TestKey::diapozon_test_2() << std::endl;
    std::cout << "3: " << TestKey::diapozon_test_3() << std::endl;
    std::cout << "4: " << TestKey::diapozon_test_4() << std::endl;
    std::cout << "5: " << TestKey::diapozon_test_5() << std::endl;
}