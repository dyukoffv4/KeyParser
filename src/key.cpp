#include "key.hpp"

/// @brief Create short key
/// @param s_data Short name [char]
keyparser::Key::Key(const char &s_data) {
    if (s_data == '~') throw std::invalid_argument("# Key.Key: Name '~' has been reserved!");
    if (s_data == '-') throw std::invalid_argument("# Key.Key: Name of key can't be empty!");
    this->s_data = s_data;
}

/// @brief Create long key
/// @param l_data Long name [string]
keyparser::Key::Key(const std::string &l_data) {
    if (l_data.size() < 2 || l_data[0] == '-')
        throw std::invalid_argument("# Key.Key: Name of key can't be empty or short!");
    this->s_data = '-';
    this->l_data = l_data;
}

/// @brief Create full key
/// @param s_data Short name [char]
/// @param l_data Long name [string]
keyparser::Key::Key(const char &s_data, const std::string &l_data) {
    if (s_data == '~') throw std::invalid_argument("# Key.Key: Name '~' has been reserved!");
    if (l_data.size() < 2 || l_data[0] == '-')
        throw std::invalid_argument("# Key.Key: Name of key can't be empty or short!");
    this->s_data = s_data;
    this->l_data = l_data;
}

bool keyparser::Key::operator<(const Key& key) const {
    if (s_data == key.s_data) return l_data < key.l_data;
    return s_data < key.s_data;
}

bool keyparser::Key::operator==(const Key& key) const {
    return s_data == key.s_data && l_data == key.l_data;
}

bool keyparser::Key::operator!=(const Key& key) const {
    return s_data != key.s_data || l_data != key.l_data;
}

/// @brief Check if keys are partially equal
/// @param key Key [Key]
bool keyparser::Key::operator^=(const Key& key) const {
    if (key.full() && full()) return s_data == key.s_data && l_data == key.l_data;
    return s_data == key.s_data || l_data == key.l_data;
}

/// @brief Check if key is full
bool keyparser::Key::full() const {
    return !l_data.empty() && s_data != '-';
}

/// @brief Return short name
char keyparser::Key::sname() const {
    return s_data;
}

/// @brief Return long name
std::string keyparser::Key::lname() const {
    return l_data;
}

/// @brief Return full name
std::string keyparser::Key::fname() const {
    if (s_data != '-' && !l_data.empty()) return std::string() + s_data + "/" + l_data;
    if (s_data == '-' && !l_data.empty()) return l_data;
    return std::string() + s_data;
}

keyparser::Key keyparser::Key::getRoot() {
    Key key('N');
    key.s_data = '~';
    return key;
}