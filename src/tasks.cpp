#include "tasks.hpp"

keyparser::Tasks::Tasks() {
    push = &root;
}

void keyparser::Tasks::pushKey(const Key& key) {
    if (!push) return;
    if (key == Key::getRoot()) push = &root;
    else {
        keys.push_back({key, Tasks()});
        push = &keys.back().second.root;
    }
}

bool keyparser::Tasks::popKey() {
    if (!push) return true;
    if (keys.empty()) return false;
    keys.pop_back();
    push = &root;
    return true;
}

void keyparser::Tasks::pushArg(const std::string& arg) {
    if (!push) return;
    push->push_back(arg);
}

bool keyparser::Tasks::popArg() {
    if (!push) return true;
    if (push->empty()) return false;
    push->pop_back();
    return true;
}

void keyparser::Tasks::lock() {
    push = nullptr;
}