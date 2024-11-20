#pragma once

#include "key.hpp"

namespace keyparser {
	class Task {
	public:
		std::vector<Task> childs;
		Key name;
		Args root;

		Task(const Key& key = Key::getRoot());

		void addKey(const Key& key);
		bool popKey(const Key& key = Key::getRoot());

		void addArg(const std::string& arg);
		bool popArg(int size = 1, bool front = false);

		int keynum() const;
		int argnum() const;
	};
}