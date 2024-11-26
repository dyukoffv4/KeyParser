#pragma once

#include "key.hpp"

namespace keyparser {
	class Task {
	public:
		std::vector<Task> childs;
		Key name;
		Args root;

		/// @brief Create root empty task
		Task(const Key& key = Key::getRoot());

		/// @brief Add new child task from key
		/// @param key Key [Key]
		void addKey(const Key& key);

		/// @brief Remove child task (last by default)
		/// @param key Key to remove [Key] (optionaly)
		/// @return False if there was no specific child task [bool]
		bool popKey(const Key& key = Key::getRoot());

		/// @brief Add parameter at root parameters
		/// @param arg Parameter [string]
		void addArg(const std::string& arg);

		/// @brief Remove root parameters from back
		/// @param size Number of parameters to remove [int] (optionaly)
		/// @param front Remove root parameters from front [bool] (optionaly)
		/// @return False if there was no parameters [bool]
		bool popArg(int size = 1, bool front = false);

		/// @brief Return number of child keys
		int keynum() const;

		/// @brief Return number of root parameters
		int argnum() const;
	};
}