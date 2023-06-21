#include "tasks.hpp"

keyparser::Tasks::Tasks(const Key& key) : r_key(key) {
    push = {&r_key, &r_args};
}

bool keyparser::Tasks::pushKey(const Key& key) {
    // push key
}

bool keyparser::Tasks::popKey() {
    if (data.empty()) return false;
    data.erase(--data.end());
    return true;
}

bool keyparser::Tasks::pushArg(const std::string& arg) {
    if ((*push.first)[push.second->size() + 1] == Key::HG) {
        if (push.first == &r_key || r_key[r_args.size() + 1] == Key::HG) return false;
        push = {&r_key, &r_args};
    }
    push.second->push_back(arg);
    return true;
}

keyparser::Tasks& keyparser::Tasks::getKeyTasks(const Key& key) {
    for (auto &i : data) if (i.first == key) return i.second;
    throw std::invalid_argument("");
}

keyparser::Tasks& keyparser::Tasks::getLastTasks() {
    return data.empty() ? *this : data.back().second;
}