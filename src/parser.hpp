#pragma once

#include <iostream>
#include "key.hpp"

namespace keyparser {
	class Parser {
	private:
		std::vector<Key> keys;

	public:
		Parser();

		Parser& operator=(const Parser& parser);

		void addKey(const char& s_data, int f_num = -1, int s_num = -1);
		void addKey(const std::string& l_data, int f_num = -1, int s_num = -1);
		void addKey(const char& s_data, const std::string& l_data, int f_num = -1, int s_num = -1);
		
		template<class T>
		void delKey(const T& data);

		template<class T>
		Key getKey(const T& data);

		void setupRoot(int f_num, int s_num);
		
		Tasks parse(int argc, char* argv[]);
		Tasks parse(Args input);
	};
}