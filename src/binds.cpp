#include "binds.hpp"

keyparser::Binds::Binds(void (*def)(const Args&)) {
    fullkeys.insert({Key::getRoot(), Key::getRoot()});
    listeners[Key::getRoot()] = def;
}

keyparser::Binds& keyparser::Binds::operator=(const Binds& binds) {
    fullkeys.clear();
    listeners.clear();
    for (auto &i : binds.fullkeys) fullkeys.insert({i.first, i.second});
    for (auto &i : binds.listeners) listeners[i.first] = i.second;
    return *this;

}

void keyparser::Binds::addListener(const Key& data, void (*def)(const Args&)) {
    if (data.full()) {
        fullkeys.insert({Key(data.s_data), data});
        fullkeys.insert({Key(data.l_data), data});
    }
    else fullkeys.insert({data, data});
    listeners[data] = def;
}

void keyparser::Binds::delListener(const Key& data) {
    if (data.full() && listeners.count(data)) {
        listeners.erase(data);
        fullkeys.erase(Key(data.s_data));
        fullkeys.erase(Key(data.l_data));
    }
    if (!data.full() && fullkeys.count(data)) {
        listeners.erase(fullkeys.find(data)->second);
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