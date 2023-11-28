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

        bool pushKey(const Key& key);
		bool popKey();

		bool pushArg(const std::string& arg);
		bool popArg();

		void lock();
	};
}