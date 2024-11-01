#include "tasks.hpp"

/// @brief Create root empty task
keyparser::Task::Task() : name(Key::getRoot()) {}

/// @brief Create root task
/// @param args Root parameters [vector<string>]
/// @param tasks Child tasks [vector<pair<Key, Task>>]
keyparser::Task::Task(Args args, TaskTree tasks) : name(Key::getRoot()), root(args) {
    for (int i = 0; i < tasks.size(); i++) {
        childs.push_back({tasks[i].first, Task(tasks[i].second.root, tasks[i].second.childs)});
        childs.back().second.name = tasks[i].first;
    }
}

/// @brief Add new child task from key
/// @param key Key [Key]
void keyparser::Task::addKey(const Key& key) {
    childs.push_back({key, Task()});
    childs.back().second.name = key;
}

/// @brief Remove last child task
/// @return False if there was no child tasks [bool]
bool keyparser::Task::popKey() {
    if (childs.empty()) return false;
    childs.pop_back();
    return true;
}

/// @brief Add parameter at root parameters
/// @param arg Parameter [string]
void keyparser::Task::addArg(const std::string& arg) {
    root.push_back(arg);
}

/// @brief Remove last root parameter
/// @return False if there was no parameters [bool]
bool keyparser::Task::popArg() {
    if (root.empty()) return false;
    root.pop_back();
    return true;
}

/// @brief Return number of root parameters
int keyparser::Task::argnum() {
    return root.size();
}