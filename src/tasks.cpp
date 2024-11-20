#include "keyparser/tasks.hpp"

/// @brief Create root empty task
keyparser::Task::Task(const Key& key) : name(key) {}

/// @brief Add new child task from key
/// @param key Key [Key]
void keyparser::Task::addKey(const Key& key) {
    childs.push_back(Task(key));
}

/// @brief Remove child task (last by default)
/// @param key Key to remove [Key] (optionaly)
/// @return False if there was no specific child task [bool]
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

/// @brief Add parameter at root parameters
/// @param arg Parameter [string]
void keyparser::Task::addArg(const std::string& arg) {
    root.push_back(arg);
}

/// @brief Remove root parameters from back
/// @param size Number of parameters to remove [int] (optionaly)
/// @param front Remove root parameters from front [bool] (optionaly)
/// @return False if there was no parameters [bool]
bool keyparser::Task::popArg(int size, bool front) {
    if (root.empty()) return false;
    if (size >= root.size()) root.clear();
    else {
        if (front) root.erase(root.begin(), root.begin() + size);
        else root.erase(root.end() - size, root.end());
    }
    return true;
}

/// @brief Return number of child keys
int keyparser::Task::keynum() const {
    return childs.size();
}

/// @brief Return number of root parameters
int keyparser::Task::argnum() const {
    return root.size();
}