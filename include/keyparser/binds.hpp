#pragma once

#include "tasks.hpp"

namespace keyparser {
    /// @brief Store binds for keys and run callback functions over task tree
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
        /// @param call_first Handle key berore root key [bool]
		/// @return Pointer to key bind [Binds*]
        Binds* bind(const Key& key, void(*new_bind)(const Args& args), bool call_first = false);

        /// @brief Add root key bind
        /// @param new_bind Handler [void(*)(const Args&)]
		/// @return Pointer to key bind [Binds*]
        Binds* bind(void(*new_bind)(const Args& args));

        /// @brief Delete key bind
        /// @param key Key [Key&]
        /// @param exact Matching parameter [bool]
		/// @return Pointer to key bind [Binds*]
        Binds* get_bind(const Key& key, bool exact = true);

        /// @brief Delete key bind
        /// @param key Key [Key&]
        /// @param exact Matching parameter [bool]
		/// @return The number of unbinds [unsigned]
        unsigned unbind(const Key& key, bool exact = true);

        /// @brief Delete root key bind
		/// @return The number of unbinds [unsigned]
        unsigned unbind();

        /// @brief Set function, running before or after handling keys
        /// @param function Handler [void(*)()]
        /// @param run_after Set handler running after all [bool]
        void bind_around(void(*function)(), bool run_after = false);

        /// @brief Run handling over tasks
        /// @param task Task to handle [Task&]
        /// @param full_key_match Matching parameter [bool]
        void execute(const Task& task, bool full_key_match = false);
    };
}