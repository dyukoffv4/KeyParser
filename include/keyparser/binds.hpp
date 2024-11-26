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
        /// @brief Create binds with root handler
        /// @param root_bind Handler [void(*)(const Args&)]
        Binds(void(*root_bind)(const Args& args) = nullptr);

        /// @brief Add key bind
        /// @param key Key [Key&]
        /// @param new_bind Handler [void(*)(const Args&)]
        /// @param call_first 
        void bind(const Key& key, const Binds& new_bind, bool call_first = false);

        /// @brief Add root key bind
        /// @param new_bind Handler [void(*)(const Args&)]
        void bind(void(*new_bind)(const Args& args));

        /// @brief Delete key bind
        /// @param key Key [Key&]
        void unbind(const Key& key);

        /// @brief Delete root key bind
        void unbind();

        /// @brief Set function, running before handling keys
        /// @param function Function [void(*)()]
        void before_all(void(*function)());

        /// @brief Set function, running after handling keys
        /// @param function Function [void(*)()]
        void after_all(void(*function)());

        /// @brief Run handling over tasks
        /// @param task Task to handle [Task&]
        /// @param full_key_match Matching parameter [bool]
        void execute(const Task& task, bool full_key_match = false);
    };
}