#pragma once

#include "tasks.hpp"

namespace keyparser {
	/// @brief Thrown to report invalid arguments to function with error index
	class trace_argument_error : public std::invalid_argument {
	public:
		int label;
		trace_argument_error(std::string msg, int label) : std::invalid_argument(msg), label(label) {}
	};

	/// @brief Store parsing settings and create task tree from input arguments
	class Parser {
	private:
		std::map<Key, Parser> parsers;
		unsigned range_0, range_1;
		
		enum argType{ARG, SKEY, LKEY};
		std::pair<int, int> static checkArg(const std::string& arg);

		void upgradeTasks(Task& tasks);

	public:
		/// @brief Create Parser with limited number of root parameters
		/// @param f_num Lower limit of parameters [unsigned]
		/// @param s_num Upper limit of parameters [unsigned]
		Parser(unsigned f_num, unsigned s_num);

		/// @brief Create parser with fixed number of parameters
		/// @param num Number of parameters [unsigned]
		Parser(unsigned num);

		/// @brief Create parser with unlimited number of parameters
		Parser();

		/// @brief Assing new parser to current
		/// @param parser Parser [Parser&]
		/// @return Reference to parser [Parser&]
		Parser& operator=(const Parser& parser);

		/// @brief Add key with limited number of parameters
		/// @param data Key [Key&]
		/// @param f_num Lower limit of parameters [unsigned]
		/// @param s_num Upper limit of parameters [unsigned]
		/// @return Pointer to key parser [Parser*]
		Parser* addKey(const Key& data, unsigned f_num, unsigned s_num);

		/// @brief Add key with fixed number of parameters
		/// @param data Key [Key&]
		/// @param num Number of parameters [unsigned]
		/// @return Pointer to key parser [Parser*]
		Parser* addKey(const Key& data, unsigned num);

		/// @brief Add key with fixed number of parameters
		/// @param data Key [Key&]
		/// @return Pointer to key parser [Parser*]
		Parser* addKey(const Key& data);
		
		/// @brief Get a key from storage
		/// @param data Key to get [Key&]
		/// @param exact Matching parameter [bool]
		/// @return Pointer to key parser [Parser*]
		Parser* getKey(const Key& data, bool exact = true);

		/// @brief Delete a key from storage
		/// @param data Key to delete [Key&]
		/// @param exact Matching parameter [bool]
		/// @return The number of elements erased [unsigned]
		unsigned delKey(const Key& data, bool exact = true);

		/// @brief Set range of numbers for root parameters
		/// @param f_num Lower limit of parameters [unsigned]
		/// @param s_num Upper limit of parameters [unsigned]
		void setRange(unsigned f_num, unsigned s_num);

		/// @brief Set fixed number for root parameters
		/// @param num Number of parameters [unsigned]
		void setRange(unsigned num);

		/// @brief Parses keys and arguments according to the specified settings
		/// @param argc Number of arguments [unsigned]
		/// @param argv Array of arguments [char**]
		/// @return Object with structured keys and parametrs for each key [Task]
		Task parse(int argc, char* argv[]);

		/// @brief Parses keys and arguments according to the specified settings
		/// @param input List of arguments to parse [vector<string>]
		/// @return Object with structured keys and parametrs for each key [Task]
		Task parse(const Args& input);

		/// @brief Parses keys and arguments according to base syntax
		/// @param input List of arguments to parse [vector<string>]
		/// @return Object with structured keys and parametrs for each key [Task]
		Task static dumbParse(const Args& input);
	};
}