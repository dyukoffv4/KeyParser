#pragma once

#include "defines.hpp"

namespace keyparser {
	class Key {
	private:
		char s_data;
		std::string l_data;

		static Key getRoot();

	public:
		enum rangeState{LW, IN, HG};

		explicit Key(const char& s_data);
		explicit Key(const std::string& l_data);
		explicit Key(const char& s_data, const std::string& l_data);

		bool operator<(const Key& key) const;
		bool operator==(const Key& key) const;

		bool full() const;

		char sname() const;
		std::string lname() const;
		std::string fname() const;

		friend class Parser;
		friend class Tasks;
	};
}