#pragma once

#include "key.hpp"
#include <map>

namespace keyparser {
	typedef std::vector<std::pair<Key, Task>> TaskTree;

	class Task {
	public:
		TaskTree childs;
		Key name;
		Args root;

		Task();
		Task(Args args, TaskTree tasks = {});

		void addKey(const Key& key);
		bool popKey();

		void addArg(const std::string& arg);
		bool popArg();

		int argnum();
	};
}