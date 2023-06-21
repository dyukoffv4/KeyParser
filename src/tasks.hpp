#pragma once

#include "key.hpp"

namespace keyparser {
	class Tasks {
	private:
		std::pair<Key*, Args*> push;

	public:
		std::vector<std::pair<Key, Tasks>> data;
		Args r_args;
		Key r_key;

		Tasks(const Key& key);

        bool pushKey(const Key& key);
		bool popKey();
		bool pushArg(const std::string& arg);

		Tasks& getKeyTasks(const Key& key);
		Tasks& getLastTasks();
	};
}