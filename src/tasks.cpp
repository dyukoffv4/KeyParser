#include "tasks.hpp"

keyparser::Tasks::Tasks() {
    push = &root;
}

void keyparser::Tasks::pushKey(const Key& key) {
    if (key == Key::getRoot()) push = &root;
    else {
        keys.push_back({key, Tasks()});
        push = &keys.back().second.root;
    }
}

bool keyparser::Tasks::popKey() {
    if (keys.empty()) return false;
    keys.pop_back();
    push = &root;
    return true;
}

void keyparser::Tasks::pushArg(const std::string& arg) {
    push->push_back(arg);
}

bool keyparser::Tasks::popArg() {
    if (push->empty()) return false;
    push->pop_back();
    return true;
}

keyparser::Tasks& keyparser::Tasks::getKeyTasks(const Key& key) {
    for (auto &i : keys) if (i.first == key) return i.second;
    throw std::invalid_argument("# Tasks.getKeyTasks: Key \"" + key.fname() + "\" doesn't exist!");
}