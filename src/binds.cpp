#include "include/keyparser/binds.hpp"

keyparser::Binds::Binds(void(*root_bind)(const Args& args)) : root_bind(root_bind) {
    before = nullptr;
    after = nullptr;
}

void keyparser::Binds::bind(Key key, const Binds& _bind, bool call_first) {
    if (call_first) {
        if (after_root_keys.count(key)) after_root_keys.erase(key);
        before_root_keys[key] = _bind;
    }
    else {
        if (before_root_keys.count(key)) before_root_keys.erase(key);
        after_root_keys[key] = _bind;
    }
}

void keyparser::Binds::bind(void(*_bind)(const Args& args)) {
    root_bind = _bind;
}

void keyparser::Binds::unbind(Key key) {
    if (after_root_keys.count(key)) after_root_keys.erase(key);
    if (before_root_keys.count(key)) before_root_keys.erase(key);
}

void keyparser::Binds::unbind() {
    root_bind = nullptr;
}

void keyparser::Binds::before_all(void(*function)()) {
    before = function;
}

void keyparser::Binds::after_all(void(*function)()) {
    after = function;
}

void keyparser::Binds::execute(const Task& task, bool full_key_match) {
    if (before) before();
    if (!before_root_keys.empty()) {
        if (full_key_match) {
            for (auto& i : task.childs) {
                if (before_root_keys.count(i.name)) before_root_keys[i.name].execute(i);
            }
        }
        else {
            for (auto& i : task.childs) {
                for (auto& j : before_root_keys) {
                    if (j.first ^= i.name) j.second.execute(i);
                }
            }
        }
    }
    if (root_bind) root_bind(task.root);
    if (!after_root_keys.empty()) {
        if (full_key_match) {
            for (auto& i : task.childs) {
                if (after_root_keys.count(i.name)) after_root_keys[i.name].execute(i);
            }
        }
        else {
            for (auto& i : task.childs) {
                for (auto& j : after_root_keys) {
                    if (j.first ^= i.name) j.second.execute(i);
                }
            }
        }
    }
    if (after) after();
}