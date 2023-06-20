#pragma once

#include <string>
#include <vector>
#include <exception>
#include <stdexcept>

namespace keyparser {
    class Key;
    class Parser;
    class Binder;

    typedef std::vector<std::string> Args;
    typedef std::vector<std::pair<Key, Args>> Tasks;
}