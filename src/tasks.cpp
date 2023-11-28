#include "tasks.hpp"

keyparser::Tasks::Tasks() {
    push = &root;
}

bool keyparser::Tasks::pushKey(const Key& key) {
    if (!push) return false;
    if (key == Key::getRoot()) push = &root;
    else {
        keys.push_back({key, Tasks()});
        push = &keys.back().second.root;
    }
    return true;
}

bool keyparser::Tasks::popKey() {
    if (!push || keys.empty()) return false;
    keys.pop_back();
    push = &root;
    return true;
}

bool keyparser::Tasks::pushArg(const std::string& arg) {
    if (!push) return false;
    push->push_back(arg);
    return true;
}

bool keyparser::Tasks::popArg() {
    if (!push || push->empty()) return false;
    push->pop_back();
    return true;
}

void keyparser::Tasks::lock() {
    push = nullptr;
}