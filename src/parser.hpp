#pragma once

#include <iostream>
#include "tasks.hpp"

namespace keyparser {
	class Parser {
	private:
		std::map<Key, Parser*> parsers;
		std::map<Key, std::pair<int, int>> ranges;
		std::map<Key, Key> fullkeys;
		
		enum argType{ARG, SKEY, LKEY};
		std::pair<int, int> static checkArg(const std::string& arg);

		enum zoneType{LW, IN, HG};
		int static checkZone(unsigned number, std::pair<int, int> zone);

		std::pair<Task, int> hardParse(const Args& input, int curr, int level, Task* parent);

	public:
		Parser(int f_num, int s_num);
		Parser(int f_num = -1);

		Parser& operator=(const Parser& parser);

		void addKey(const Key& data, int f_num, int s_num);
		void addKey(const Key& data, int f_num);
		void addKey(const Key& data, Parser* parser = nullptr);
		void delKey(const Key& data);

		void setArgnum(int f_num, int s_num);
		void setArgnum(int f_num);
		
		Task parse(int argc, char* argv[]);
		Task parse(Args input);
	};
}