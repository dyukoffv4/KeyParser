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
		unsigned range_0, range_1;
		
		enum argType{ARG, SKEY, LKEY};
		std::pair<int, int> static checkArg(const std::string& arg);

		void upgradeTasks(Task& tasks);

	public:
		Parser(unsigned f_num, unsigned s_num);
		Parser(unsigned num);
		Parser();

		Parser& operator=(const Parser& parser);

		Parser& addKey(const Key& data, unsigned f_num, unsigned s_num);
		Parser& addKey(const Key& data, unsigned num);
		Parser& addKey(const Key& data);
		
		Parser& getKey(const Key& data);

		void delKey(const Key& data);

		void setRange(unsigned f_num, unsigned s_num);
		void setRange(unsigned num);

		Task parse(int argc, char* argv[]);
		Task parse(const Args& input);

		Task static dumbParse(const Args& input);
	};
}