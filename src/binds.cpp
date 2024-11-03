#include "binds.hpp"

keyparser::Binds::Binds(void (*def)(const Args&)) {
    fullkeys[Key::getRoot()] = Key::getRoot();
    listeners[Key::getRoot()] = def;
}

keyparser::Binds& keyparser::Binds::operator=(const Binds& binds) {
    fullkeys.clear();
    listeners.clear();
    for (auto &i : binds.fullkeys) fullkeys[i.first] = i.second;
    for (auto &i : binds.listeners) listeners[i.first] = i.second;
}

void keyparser::Binds::addListener(const Key& data, void (*def)(const Args&)) {
    fullkeys.insert({Key(data.s_data), data});
    fullkeys.insert({Key(data.l_data), data});
    listeners[data] = def;
}

void keyparser::Binds::delListener(const Key& data) {
    if (!listeners.count(data) && !fullkeys.count(data)) return;
    if (data.full()) {
        listeners.erase(data);
        fullkeys.erase(Key(data.s_data));
        fullkeys.erase(Key(data.l_data));
    }
    else {
        listeners.erase(fullkeys[data]);
        fullkeys.erase(data);
    }
}

void keyparser::Binds::addSelfListener(void (*def)(const Args&)) {
    listeners[Key::getRoot()] = def;
}

void keyparser::Binds::delSelfListener() {
    listeners[Key::getRoot()] = nullptr;
}

void keyparser::Binds::execute(const Task& task) {

}