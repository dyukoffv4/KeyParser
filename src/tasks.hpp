#pragma once

#include "key.hpp"

namespace keyparser {
	class Tasks {
	private:
		Args* push;

	public:
		std::vector<std::pair<Key, Tasks>> keys;
		Args root;

		Tasks();

        void pushKey(const Key& key);
		bool popKey();

		void pushArg(const std::string& arg);
		bool popArg();

		Tasks& getKeyTasks(const Key& key);
	};
}