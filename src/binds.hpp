#pragma once

#include "tasks.hpp"

namespace keyparser {
	class Binds {
	private:
		std::map<Key, void (*)(const Args&)> listeners;
		std::map<Key, Key> fullkeys;

	public:
		Binds(void (*def)(const Args&) = nullptr);

		Binds& operator=(const Binds& binds);

		void addListener(const Key& data, void (*def)(const Args&));
		void delListener(const Key& data);

        void addSelfListener(void (*def)(const Args&));
        void delSelfListener();

        void execute(const Task& task);
	};
}