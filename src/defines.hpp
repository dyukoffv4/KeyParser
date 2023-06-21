#pragma once

#include <string>
#include <vector>
#include <exception>
#include <stdexcept>

namespace keyparser {
    class Key;
    class Parser;
    class Binder;
    class Tasks;

    typedef std::vector<std::string> Args;
    enum argType{ARG, AKEY, SKEY, LKEY};

    int checkArgument(std::string arg) {
        if (arg.empty()) throw std::invalid_argument("# Parser.parser: Empty argument were got!\n");
        if (arg[0] == '-') {
            if (arg.size() == 2) {
                if (arg[1] != '-') return 2;
                throw std::invalid_argument("# Parser.parser: Key expected after \"--\"!\n");
            }
            if (arg.size() > 2) {
                if (arg[1] == '-') return (arg[2] == '-') ? 1 : 3;
                throw std::invalid_argument("# Parser.parser: Short key expected after \"-\"!\n");
            }
            throw std::invalid_argument("# Parser.parser: Key expected after \"-\"!\n");
        }
        return 0;
    }
}