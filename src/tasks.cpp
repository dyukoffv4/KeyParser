#include "tasks.hpp"

keyparser::Task::Task() : name(Key::getRoot()) {}

keyparser::Task::Task(Args args, TaskTree tasks) : name(Key::getRoot()), root(args) {
    for (int i = 0; i < tasks.size(); i++) {
        childs.push_back({tasks[i].first, Task(tasks[i].second.root, tasks[i].second.childs)});
        childs.back().second.name = tasks[i].first;
    }
}

void keyparser::Task::addKey(const Key& key) {
    childs.push_back({key, Task()});
    childs.back().second.name = key;
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