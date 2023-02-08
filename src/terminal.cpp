#include "terminal.hpp"

KP::Terminal::Terminal(rootState state) : state(state) {
    binds[Key::getRoot()] = nullptr;
    binds[Key::getNull()] = nullptr;
    last = nullptr;
}

KP::Terminal &KP::Terminal::operator=(const Terminal &term) {
    this->state = term.state;
    this->last = term.last;
    binds.clear();
    for (auto &i : term.binds) binds[i.first] = i.second;
    return *this;
}

void KP::Terminal::setKey(Key key, void (*lnr)(Args)) {
    delKey(key);
    if (key.getState() == Key::State::A) {
        binds[Key(key.lname())] = lnr;
        binds[Key(key.sname())] = lnr;
    }
    else binds[key] = lnr;
}

void KP::Terminal::delKey(Key key) {
    if (key.getState() == Key::State::A) {
        if (binds.count(Key(key.lname()))) binds.erase(binds.find(Key(key.lname())));
        if (binds.count(Key(key.sname()))) binds.erase(binds.find(Key(key.sname())));
    }
    else if (binds.count(key)) binds.erase(binds.find(key));
}

void KP::Terminal::setRootRange(int f_num, int s_num) {
    void (*lnr)(Args) = binds.find(Key::getRoot())->second;
    binds.erase(binds.find(Key::getRoot()));
    binds[Key::getRoot(f_num, s_num)] = lnr;
}

void KP::Terminal::setRootState(rootState state) {
    this->state = state;
}

void KP::Terminal::setRoot(void (*lnr)(Args)) {
    binds[Key::getRoot()] = lnr;
}

void KP::Terminal::delRoot() {
    binds[Key::getRoot()] = nullptr;
}

void KP::Terminal::setFinal(void (*lnr)(void)) {
    last = lnr;
}

void KP::Terminal::delFinal() {
    last = nullptr;
}

void KP::Terminal::cleanBinds() {
    Key r_key = binds.find(Key::getRoot())->first;
    Key n_key = binds.find(Key::getNull())->first;
    binds.clear();
    binds[r_key] = nullptr;
    binds[n_key] = nullptr;
}

void KP::Terminal::execute(int argc, char* argv[]) {
    Args data;
	for (int i = 0; i < argc; i++) data.push_back(argv[i]);
    execute(data);
}

void KP::Terminal::execute(Args input) {
    std::vector<std::pair<Key, Args>> tasks = {{binds.find(Key::getRoot())->first, Args()}};

    // Arguments parsing
    for (auto &i : input) {
        if ((i.size() == 3 && i[1] == '-') || (i.size() >= 3 && i.substr(0, 2) == "--") || i[0] != '-') {
            if (i[0] == '-') tasks[tasks.size() - 1].second.push_back(i.substr(1));
            else {
                int esize = tasks[tasks.size() - 1].second.size() + 1;
                if (tasks[tasks.size() - 1].first[esize] == Key::zoneState::ZS_H) tasks.push_back({binds.find(Key::getRoot())->first, Args()});
                tasks[tasks.size() - 1].second.push_back(i);
            }
        }
        else {
            if (i.size() == 1) std::cout << "# Terminal.execute: Key expected after \"-\"!\n";
            else {
                if (i.size() > 1 && i[1] == '-') {
                    if (i.size() == 2) std::cout << "# Terminal.execute: Key expected after \"--\"!\n";
                    else {
                        auto key = binds.find(Key(i.substr(2)));
                        if (key != binds.end()) tasks.push_back({key->first, Args()});
                        else std::cout << "# Terminal.execute: Key with name \"" << i.substr(2) << "\" doesn't exist!\n";
                    }
                }
                else {
                    if (i.size() > 2) std::cout << "# Terminal.execute: Short key expected after \"-\"!\n";
                    else {
                        auto key = binds.find(Key(i[1]));
                        if (key != binds.end()) tasks.push_back({key->first, Args()});
                        else std::cout << "# Terminal.execute: Key with name \"" << i[1] << "\" doesn't exist!\n";
                    }
                }
            }
        }
    }

    // Tasks execute
    for (int i = 1; i < tasks.size(); i++) {
        if (tasks[i].first == Key::getRoot()) {
            if (state == RS_S) tasks[0].second.insert(tasks[0].second.end(), tasks[i].second.begin(), tasks[i].second.end());
            if (state != RS_A) tasks.erase(tasks.begin() + i--);
        }
    }
    if (state == RS_S) {
        tasks.push_back(tasks[0]);
        tasks.erase(tasks.begin());
    }
    for (auto &i : tasks) {
        if (i.first[i.second.size()] == Key::zoneState::ZS_I) {
            try {
                if (binds[i.first]) binds[i.first](i.second);
            }
            catch (std::invalid_argument e) {
                std::cout << "# Terminal.execute->" << e.what() << "\n";
            }
        }
        else std::cout << "# Terminal.execute: Key " << i.first.fname() << " could minimum take " << i.first.lk_num << " parametrs!\n";
    }
    try {
        if (last) last();
    }
    catch (std::invalid_argument e) {
        std::cout << "# Terminal.execute->" << e.what() << "\n";
    }
}