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

		Task(Args args = {});	
		Task(Key key, Args args = {});

		void addKey(const Key& key);
		bool popKey();

		void addArg(const std::string& arg);
		bool popArg();

		Key getkey();
		int argnum();
	};
}