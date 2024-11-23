#pragma once

#include "tasks.hpp"

namespace keyparser {
    class Binds {
    private:
        void(*before)();
        void(*after)();
        void(*root_bind)(const Args& args);
        std::map<Key, Binds> before_root_keys;
        std::map<Key, Binds> after_root_keys;

    public:
        Binds(void(*root_bind)(const Args& args) = nullptr);

        void bind(Key key, const Binds& _bind, bool call_first = false);
        void bind(void(*_bind)(const Args& args));

        void unbind(Key key);
        void unbind();

        void before_all(void(*function)());
        void after_all(void(*function)());

        void execute(const Task& task, bool full_key_match = false);
    };
}
