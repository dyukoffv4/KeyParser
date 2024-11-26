#include "include/keyparser/tasks.hpp"

keyparser::Task::Task(const Key& key) : name(key) {}

void keyparser::Task::addKey(const Key& key) {
    childs.push_back(Task(key));
}

bool keyparser::Task::popKey(const Key& key) {
    if (childs.empty()) return false;
    if (key == Key::getRoot()) {
        childs.pop_back();
        return true;
    }
    for (auto i = childs.begin(); i != childs.end(); i++) if (i->name ^= key) {
        childs.erase(i);
        return true;
    }
    return false;
}

void keyparser::Task::addArg(const std::string& arg) {
    root.push_back(arg);
}

bool keyparser::Task::popArg(int size, bool front) {
    if (root.empty()) return false;
    if (size >= root.size()) root.clear();
    else {
        if (front) root.erase(root.begin(), root.begin() + size);
        else root.erase(root.end() - size, root.end());
    }
    return true;
}

int keyparser::Task::keynum() const {
    return childs.size();
}

int keyparser::Task::argnum() const {
    return root.size();
}