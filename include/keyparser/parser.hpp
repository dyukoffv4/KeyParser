#pragma once

#include "tasks.hpp"

namespace keyparser {
	class trace_argument_error : public std::invalid_argument {
	public:
		int label;
		trace_argument_error(std::string msg, int label) : std::invalid_argument(msg), label(label) {}
	};

	class Parser {
	private:
		std::map<Key, Parser> parsers;
		std::pair<int, int> range;
		
		enum argType{ARG, SKEY, LKEY};
		std::pair<int, int> static checkArg(const std::string& arg);

		enum zoneType{LW, IN, HG};
		int static checkZone(unsigned number, std::pair<int, int> zone);

		void upgradeTasks(Task& tasks);

	public:
		Parser(int f_num, int s_num);
		Parser(int f_num = -1);

		Parser& operator=(const Parser& parser);

		Parser& addKey(const Key& data, int f_num, int s_num);
		Parser& addKey(const Key& data, int f_num = -1);
		
		void delKey(const Key& data);

		void setRange(int f_num, int s_num);
		void setRange(int f_num);

		Task parse(int argc, char* argv[]);
		Task parse(Args input);

		Task static dumbParse(const Args& input);
	};
}