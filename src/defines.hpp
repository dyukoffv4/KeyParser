#pragma once

#include <string>
#include <vector>
#include <exception>
#include <stdexcept>

namespace keyparser {
    class Key;
    class Parser;
    class Tasks;

    typedef std::vector<std::string> Args;
    typedef std::vector<std::pair<Key, Tasks>> Parsed;
}