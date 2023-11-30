#pragma once

#include <iostream>
#include <map>
#include "tasks.hpp"

namespace keyparser {
	class Parser {
	private:
		std::map<Key, Parser*> parsers;
		std::map<Key, std::pair<int, int>> ranges;
		std::map<Key, Key> fullkeys;
		
		enum argType{ARG, AKEY, SKEY, LKEY};
		int static checkArgument(const std::string& arg);

		enum zoneType{LW, IN, HG};
		int static checkZoneStat(unsigned number, std::pair<int, int> zone);

	public:
		Parser(int f_num, int s_num);
		Parser(int f_num = -1);

		Parser& operator=(const Parser& parser);

		void addKey(const Key& data, int f_num, int s_num, Parser* parser = nullptr);
		void addKey(const Key& data, int f_num, Parser* parser = nullptr);
		void addKey(const Key& data, Parser* parser = nullptr);
		void delKey(const Key& data);

		void setKeyParser(const Key& key, Parser* parser);
		void setKeyArgnum(const Key& key, int f_num, int s_num);
		void setKeyArgnum(const Key& key, int f_num);

		void setArgnum(int f_num, int s_num);
		void setArgnum(int f_num);
		
		Tasks parse(int argc, char* argv[]);
		Tasks parse(Args input);

		Tasks softParse(int argc, char* argv[]);
		Tasks softParse(Args input);
	};
}