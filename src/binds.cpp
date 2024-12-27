#include "include/keyparser/binds.hpp"

keyparser::Binds::Binds(void(*root_bind)(const Args& args)) : root_bind(root_bind) {
    before = nullptr;
    after = nullptr;
}

keyparser::Binds* keyparser::Binds::bind(const Key& key, void(*new_bind)(const Args& args), bool call_first) {
    if (get_bind(key, false)) return nullptr;
    if (call_first) return &(before_root_keys[key] = Binds(new_bind));
    return &(after_root_keys[key] = Binds(new_bind));
}

keyparser::Binds* keyparser::Binds::bind(void(*new_bind)(const Args& args)) {
    root_bind = new_bind;
}

keyparser::Binds* keyparser::Binds::get_bind(const Key& key, bool exact) {
    if (exact) {
        if (before_root_keys.count(key)) return &before_root_keys[key];
        if (after_root_keys.count(key)) return &after_root_keys[key];
        return nullptr;
    }
    for (auto i = before_root_keys.begin(); i != before_root_keys.end(); i++) {
        if (i->first ^= key) return &(i->second);
    }
    for (auto i = after_root_keys.begin(); i != after_root_keys.end(); i++) {
        if (i->first ^= key) return &(i->second);
    }
    return nullptr;
}

unsigned keyparser::Binds::unbind(const Key& key, bool exact) {
    if (exact) {
        if (before_root_keys.count(key)) {
            before_root_keys.erase(key);
            return 1;
        }
        if (after_root_keys.count(key)) {
            after_root_keys.erase(key);
            return 1;
        }
        return 0;
    }
    unsigned erased = 0;
    for (auto i = before_root_keys.begin(); i != before_root_keys.end();) {
        if (i->first ^= key) {
            i = before_root_keys.erase(i);
            erased++;
        }
        else i++;
    }
    for (auto i = after_root_keys.begin(); i != after_root_keys.end();) {
        if (i->first ^= key) {
            i = after_root_keys.erase(i);
            erased++;
        }
        else i++;
    }
    return erased;
}

unsigned keyparser::Binds::unbind() {
    root_bind = nullptr;
}

void keyparser::Binds::bind_around(void(*function)(), bool run_after) {
    if (run_after) after = function;
    else before = function;
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