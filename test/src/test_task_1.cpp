#include "../../src/parser.hpp"
#include <iostream>

using namespace keyparser;
using namespace std;

std::ostream &operator<<(std::ostream &out, const keyparser::Args &data) {
    if (data.size() == 0) return out;
    for (auto i = data.begin(); i != --data.end(); i++) out << *i << ", ";
    out << *(--data.end());
    return out;
}

void task_print(std::ostream &out, const keyparser::Task &data, std::string offset = "") {
    out << offset << data.name.fname() << " : " << data.root << '\n';
    for (auto &i : data.childs) task_print(out, i, offset + "  ");
}

bool operator==(const keyparser::Task &f, const keyparser::Task &s) {
    if (f.childs.empty() && s.childs.empty()) return (f.root == s.root && f.name == s.name);
    if (f.name != s.name || f.root != s.root || f.keynum() != s.keynum()) return false;
    for (int i = 0; i < f.childs.size(); i++) if (!(f.childs[i] == s.childs[i])) return false;
    return true;
}

int main() {
    Args input = {"-a", "1", "2", "3", "--b", "---c", "1", "---d", "--e", "1", "2", "-f", "1", "2", "3", "-g", "1", "2"};

    Task expected, result;
    expected.root = {"3"};
    expected.childs = {Task(Key('a')), Task(Key('f')), Task(Key('g'))};
    expected.childs[0].root = {"1", "2", "3"};
    expected.childs[0].childs = {Task(Key('b')), Task(Key('e'))};
    expected.childs[0].childs[0].childs = {Task(Key('c')), Task(Key('d'))};
    expected.childs[0].childs[0].childs[0].root = {"1"};
    expected.childs[0].childs[1].root = {"1", "2"};
    expected.childs[1].root = {"1", "2"};
    expected.childs[2].root = {"1", "2"};

    Parser parser, a_parser(2, 4), ab_parser(0);
    ab_parser.addKey(Key('c'), 0, 2);
    ab_parser.addKey(Key('d'), 0);

    a_parser.addKey(Key('b'), &ab_parser);
    a_parser.addKey(Key('e'), 2);

    parser.addKey(Key('a'), &a_parser);
    parser.addKey(Key('f'), 0, 2);
    parser.addKey(Key('g'), 2, 4);

    // Test

    cout << "Keys:  ~(0,:)-->[a(2,4)-->[b(0)-->[c(0,2);  d(0)];  e(2)];  f(0,2);  g(2,4)]\n\n";
    cout << "Input: " << input << "\n";
    try {
        result = parser.parse(input);
        cout << "\nComputed:\n";
        task_print(cout, result);
    }
    catch (std::invalid_argument e) {
        cout << "\nComputed:\n" << e.what() << "\n";
    }
    cout << "\nExpected:\n";
    task_print(cout, expected);
    cout << "\nResult:  " << (result == expected ? "PASS" : "ERROR") << "\n";
    
    return 0;
}