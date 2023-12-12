#include "../../src/parser.hpp"

using namespace keyparser;
using namespace std;

std::ostream& operator<<(std::ostream& out, const keyparser::Args& data) {
    if (data.size() == 0) return out;
    for (auto i = data.begin(); i != --data.end(); i++) out << *i << ", ";
    out << *(--data.end());
    return out;
}

std::ostream& operator<<(std::ostream& out, const keyparser::Task& data) {
    out << data.name.fname() << " : " << data.root << '\n';
    for (auto& i : data.childs) out << i.first.fname() << "\t:  " << i.second << '\n';
    return out;
}

bool operator==(const keyparser::Task& f, const keyparser::Task& s) {
    if (f.childs.empty() && s.childs.empty()) return f.root == s.root;
    if (f.childs.empty() || s.childs.empty() || f.childs.size() != s.childs.size()) return false;
    if (f.root != s.root) return false;
    for (int i = 0; i < f.childs.size(); i++) {
        if (!(f.childs[i].first == s.childs[i].first && f.childs[i].second == s.childs[i].second)) return false;
    }
    return true;
}

class TestParser {
public:
    static void test_1(string name) {
        std::vector<Args> input = {
            {"-a", "1", "2", "-b", "-c", "1", "2", "3", "-d", "1", "2", "3", "4", "5"},
            {"-a", "1", "2", "3", "4", "-b", "1", "2", "-c", "-d", "1", "2", "3", "4"},
            {"-d", "1", "-a", "-b", "1", "-c", "1", "2", "3", "4", "5", "6", "7", "8"},
            {"-d", "1", "2", "3", "-c", "1", "2", "-b", "-a", "1", "2", "3", "4", "5"},
            {"w", "-a", "1", "-b", "-c", "1", "2", "3", "4", "5", "-d", "1", "2", "3"}
        };

        std::vector<std::string> expected = {"1", "error", "1", "1", "1"};

        std::vector<Task> output = std::vector<Task>(5, Task());
        output[0].addKey(Key('a'));
        output[0].childs.back().second.root = {"1", "2"};
        output[0].addKey(Key('b'));
        output[0].addKey(Key('c'));
        output[0].childs.back().second.root = {"1", "2", "3"};
        output[0].addKey(Key('d'));
        output[0].childs.back().second.root = {"1", "2", "3", "4", "5"};

        output[2].addKey(Key('a'));
        output[2].addKey(Key('b'));
        output[2].addKey(Key('c'));
        output[2].childs.back().second.root = {"1", "2", "3", "4"};
        output[2].addKey(Key('d'));
        output[2].childs.back().second.root = {"1"};
        output[2].root = {"1", "5", "6", "7", "8"};

        output[3].addKey(Key('a'));
        output[3].childs.back().second.root = {"1", "2"};
        output[3].addKey(Key('b'));
        output[3].addKey(Key('c'));
        output[3].childs.back().second.root = {"1", "2"};
        output[3].addKey(Key('d'));
        output[3].childs.back().second.root = {"1", "2", "3"};
        output[3].root = {"3", "4", "5"};

        output[4].addKey(Key('a'));
        output[4].childs.back().second.root = {"1"};
        output[4].addKey(Key('b'));
        output[4].addKey(Key('c'));
        output[4].childs.back().second.root = {"1", "2", "3", "4"};
        output[4].addKey(Key('d'));
        output[4].childs.back().second.root = {"1", "2", "3"};
        output[4].root = {"w", "5"};

        Parser terminal;
        terminal.addKey(Key('a'), 0, 2);
        terminal.addKey(Key('b'), 0);
        terminal.addKey(Key('c'), 2, 4);
        terminal.addKey(Key('d'));

        cout << name << "\n\nKeys  a: 0-2,  b: 0,  c: 2-4,  d: all\n\n";
        for (int i = 0; i < input.size(); i++) {
            cout << "Test " << i + 1 << ": " << input[i] << "\n";
            cout << "Expected " << expected[i] << "\n";
            try {
                bool result = terminal.parse(input[i]) == output[i];
                cout << "Result: " << result << "\n\n";
            }
            catch (std::invalid_argument e) {
                cout << "Result: " << e.what() << "\n";
            }
        }
    }

    static void test_2(string name) {
        std::vector<Args> input = {
            {"-a", "1", "-b", "1", "--a", "1", "2", "3", "--c", "1", "2", "3"},
            {"-b", "--a", "1", "--c", "-a", "1", "2", "3", "4", "-b", "1", "2"},
            {"--b", "--a", "1", "--c", "-a", "1", "2", "3", "4", "-b", "1", "2"},
        };

        std::vector<std::string> expected = {"1", "error", "error"};

        std::vector<Task> output = std::vector<Task>(5, Task());
        output[0].addKey(Key('a'));
        output[0].childs.back().second.root = {"1"};
        output[0].addKey(Key('b'));
        output[0].childs.back().second.addKey(Key('a'));
        output[0].childs.back().second.childs.back().second.root = {"1", "2"};
        output[0].childs.back().second.addKey(Key('c'));
        output[0].childs.back().second.root = {"1", "3", "1", "2"};
        output[0].root = {"3"};

        Parser terminal, b_terminal(2, 4);
        b_terminal.addKey(Key('a'), 0, 2);
        b_terminal.addKey(Key('c'), 0);
        terminal.addKey(Key('a'), 0, 2);
        terminal.addKey(Key('b'), &b_terminal);

        cout << name << "\n\nKeys  a: 0-2,  b: 2-4, b->a: 0-2,  b->g: 0\n\n";
        for (int i = 0; i < input.size(); i++) {
            cout << "Test " << i + 1 << ": " << input[i] << "\n";
            cout << "Expected " << expected[i] << "\n";
            try {
                bool result = terminal.parse(input[i]) == output[i];
                cout << "Result: " << result << "\n\n";
            }
            catch (std::invalid_argument e) {
                cout << "Result: " << e.what() << "\n";
            }
        }
    }
};

int main() {
    TestParser::test_1("---------\nParser 1:");
    TestParser::test_2("---------\nParser 2:");
    return 0;
}