#pragma once

#include <iostream>
#include "key.hpp"
#include "tasks.hpp"

namespace keyparser {
	class Parser {
	private:
		std::vector<std::pair<Key, Parser*>> keys;

	public:
		Parser();

		Parser& operator=(const Parser& parser);

		template<class T>
		void addKey(const T& data, Parser* parser = nullptr);
		
		template<class T>
		void delKey(const T& data);

		template<class T>
		std::pair<Key, Parser*>& getKey(const T& data);

		void setupKeyParser(const Key& key, Parser* parser);
		void setupKeyArgnum(const Key& key, int f_num, int s_num = -1);
		
		Tasks parse(int argc, char* argv[]);
		Tasks parse(Args input);
	};
}