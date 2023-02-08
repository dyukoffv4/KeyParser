#include "key.hpp"

KP::Key::Key(const char &s_data, int f_num, int s_num) : lk_num(f_num), hk_num(s_num) {
    if (s_data == '~') throw std::invalid_argument("# Key.Key: Name '~' has been reserved!");
    if (s_data == '-') throw std::invalid_argument("# Key.Key: Name of key can't be empty!");
    if (f_num > s_num && s_num > -1) throw std::invalid_argument("# Key.Key: First num can't be bigger then second!");
    this->s_data = s_data;
}

KP::Key::Key(const std::string &l_data, int f_num, int s_num) : lk_num(f_num), hk_num(s_num) {
    if (l_data.empty()) throw std::invalid_argument("# Key.Key: Name of key can't be empty!");
    if (f_num > s_num && s_num > -1) throw std::invalid_argument("# Key.Key: First num can't be bigger then second!");
    this->s_data = '-';
    this->l_data = l_data;
}

KP::Key::Key(const char &s_data, const std::string &l_data, int f_num, int s_num) : lk_num(f_num), hk_num(s_num) {
    if (s_data == '~') throw std::invalid_argument("# Key.Key: Name '~' has been reserved!");
    if (l_data.empty()) throw std::invalid_argument("# Key.Key: Name of key can't be empty!");
    if (f_num > s_num && s_num > -1) throw std::invalid_argument("# Key.Key: First num can't be bigger then second!");
    this->s_data = s_data;
    this->l_data = l_data;
}

KP::Key::State KP::Key::getState() const {
    if (s_data == '-' && l_data.empty()) return State::E;
    if (l_data.empty()) return State::S;
    if (s_data == '-') return State::L;
    return State::A;
}

bool KP::Key::operator<(const Key& key) const {
    if (s_data == key.s_data) return l_data < key.l_data;
    return s_data < key.s_data;
}

bool KP::Key::operator==(const Key& key) const {
    return s_data == key.s_data && l_data == key.l_data;
}

KP::Key::zoneState KP::Key::operator[](const int& num) const {
    if (lk_num < 0 && hk_num < 0) return ZS_I;
    if (lk_num < 0) return num <= hk_num ? ZS_I : ZS_H;
    if (hk_num < 0) return num >= lk_num ? ZS_I : ZS_L;
    return num < lk_num ? ZS_L : (num >= lk_num && num <= hk_num ? ZS_I : ZS_H);
}

char KP::Key::sname() const {
    return s_data;
}

std::string KP::Key::lname() const {
    return l_data;
}

std::string KP::Key::fname() const {
    if (getState() == A) return std::string() + s_data + "/" + l_data;
    if (getState() == L) return l_data;
    return std::string() + s_data;
}

KP::Key KP::Key::getNull(int f_num, int s_num) {
    Key key('N', f_num, s_num);
    key.s_data = '-';
    return key;
}

KP::Key KP::Key::getRoot(int f_num, int s_num) {
    Key key('N', f_num, s_num);
    key.s_data = '~';
    return key;
}