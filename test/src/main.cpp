#include "../../src/parser.hpp"

using namespace keyparser;
using namespace std;

std::ostream &operator<<(std::ostream &out, const keyparser::Args &data) {
    if (data.size() == 0) return out;
    for (auto i = data.begin(); i != --data.end(); i++) out << *i << ", ";
    out << *(--data.end());
    return out;
}

char task_print(std::ostream &out, const keyparser::Task &data, std::string offset = "") {
    out << offset << data.name.fname() << " : " << data.root << '\n';
    for (auto &i : data.childs) task_print(out, i.second, offset + "  ");
    return ' ';
}

bool operator==(const keyparser::Task &f, const keyparser::Task &s) {
    if (f.childs.empty() && s.childs.empty()) return f.root == s.root;
    if (f.childs.empty() || s.childs.empty() || f.childs.size() != s.childs.size()) return false;
    if (f.root != s.root) return false;
    for (int i = 0; i < f.childs.size(); i++) {
        if (!(f.childs[i].first == s.childs[i].first && f.childs[i].second == s.childs[i].second)) return false;
    }
    return true;
}

void test(const string &header, Parser &parser, const vector<Args> &inputs, const vector<Task> &results) {
    cout << header << "\n\n";
    for (int i = 0; i < inputs.size(); i++) {
        cout << "Test " << i + 1 << ": " << inputs[i] << "\n";
        try {
            Task result = parser.parse(inputs[i]);
            cout << "Computed:\n" << task_print(cout, result) << "\n";
            cout << "Expected:\n" << task_print(cout, results[i]) << "\n";
            cout << "Result:  " << (result == results[i]) << "\n";
        }
        catch (std::invalid_argument e) {
            cout << "Computed:\n" << e.what() << "\n";
            cout << "Expected:\n" << task_print(cout, results[i]) << "\n";
            cout << "Result:  ERROR\n";
        }
    }
}

int main() {
    std::vector<Args> input = {
        {"-a", "1", "2", "3", "--b", "---c", "1", "---d", "--e", "1", "2", "-f", "1", "2", "3", "-g", "1", "2"}
    };

    std::vector<Task> output = std::vector<Task>(1);
    output[0] = Task({"3"}, {{Key('a'), Task({"1", "2", "3"})}, {Key('f'), Task({"1", "2"})}, {Key('g'), Task({"1", "2"})}});
    output[0].childs[0].second.childs = {{Key('b'), Task()}, {Key('e'), Task({"1", "2"})}};
    output[0].childs[0].second.childs[0].second.childs = {{Key('c'), Task({"1"})}, {Key('d'), Task()}};

    Parser parser, a_parser(2, 4), ab_parser(0);
    ab_parser.addKey(Key('c'), 0, 2);
    ab_parser.addKey(Key('d'), 0);

    a_parser.addKey(Key('b'), &ab_parser);
    a_parser.addKey(Key('e'), 2);

    parser.addKey(Key('a'), &a_parser);
    parser.addKey(Key('f'), 0, 2);
    parser.addKey(Key('g'), 2, 4);

    test("Keys:  ~(0,:)-->[a(2,4)-->[b(0)-->[c(0,2);  d(0)];  e(2)];  f(0,2);  g(2,4)]", parser, input, output);
    return 0;
}