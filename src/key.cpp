#include "key.hpp"

keyparser::Key::Key(const char &s_data, int f_num, int s_num) : lk_num(f_num), hk_num(s_num) {
    if (s_data == '~') throw std::invalid_argument("# Key.Key: Name '~' has been reserved!");
    if (s_data == '-') throw std::invalid_argument("# Key.Key: Name of key can't be empty!");
    this->setRange(f_num, s_num);
    this->s_data = s_data;
}

keyparser::Key::Key(const std::string &l_data, int f_num, int s_num) : lk_num(f_num), hk_num(s_num) {
    if (l_data.empty()) throw std::invalid_argument("# Key.Key: Name of key can't be empty!");
    this->setRange(f_num, s_num);
    this->s_data = '-';
    this->l_data = l_data;
}

keyparser::Key::Key(const char &s_data, const std::string &l_data, int f_num, int s_num) {
    if (s_data == '~') throw std::invalid_argument("# Key.Key: Name '~' has been reserved!");
    if (l_data.empty()) throw std::invalid_argument("# Key.Key: Name of key can't be empty!");
    this->setRange(f_num, s_num);
    this->s_data = s_data;
    this->l_data = l_data;
}

void keyparser::Key::setRange(int f_num, int s_num = -1) {
    if (f_num > s_num && s_num > -1) throw std::invalid_argument("# Key.Key: First num can't be bigger then second!");
    lk_num = s_num < 0 ? -1 : f_num;
    hk_num = s_num < 0 ? f_num : s_num;
}

bool keyparser::Key::operator==(const Key& key) const {
    if (key.l_data.empty() || l_data.empty()) return s_data == key.s_data;
    if (key.s_data == '-' || s_data == '-') return l_data == key.l_data;
    return s_data == key.s_data || l_data == key.l_data;
}

keyparser::Key::rangeState keyparser::Key::operator[](const int& num) const {
    if (lk_num < 0 && hk_num < 0) return IN;
    if (lk_num < 0) return num <= hk_num ? IN : HG;
    if (hk_num < 0) return num >= lk_num ? IN : LW;
    return num < lk_num ? LW : (num >= lk_num && num <= hk_num ? IN : HG);
}

char keyparser::Key::sname() const {
    return s_data;
}

std::string keyparser::Key::lname() const {
    return l_data;
}

std::string keyparser::Key::fname() const {
    if (s_data != '-' && !l_data.empty()) return std::string() + s_data + "/" + l_data;
    if (s_data == '-' && !l_data.empty()) return l_data;
    return std::string() + s_data;
}

keyparser::Key keyparser::Key::getRoot(int f_num, int s_num) {
    Key key('N', f_num, s_num);
    key.s_data = '~';
    return key;
}