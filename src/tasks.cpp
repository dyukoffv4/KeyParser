#include "tasks.hpp"

keyparser::Task::Task(Args args) : name(Key::getRoot()), root(args) {}

keyparser::Task::Task(Key key, Args args) : name(key), root(args) {}

void keyparser::Task::addKey(const Key& key) {
    childs.push_back({key, Task(key)});
}

bool keyparser::Task::popKey() {
    if (childs.empty()) return false;
    childs.pop_back();
    return true;
}

void keyparser::Task::addArg(const std::string& arg) {
    root.push_back(arg);
}

bool keyparser::Task::popArg() {
    if (root.empty()) return false;
    root.pop_back();
    return true;
}

keyparser::Key keyparser::Task::getkey() {
    return name;
}

int keyparser::Task::argnum() {
    return root.size();
}