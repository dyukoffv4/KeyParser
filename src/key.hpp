#pragma once

#include "defines.hpp"

namespace keyparser {
	class Key {
	private:
		char s_data;
		std::string l_data;
		int lk_num;
		int hk_num;

		static Key getRoot(int f_num = -1, int s_num = -1);

	public:
		enum rangeState{LW, IN, HG};

		explicit Key(const char& s_data, int f_num = -1, int s_num = -1);
		explicit Key(const std::string& l_data, int f_num = -1, int s_num = -1);
		explicit Key(const char& s_data, const std::string& l_data, int f_num = -1, int s_num = -1);

		bool operator==(const Key& key) const;
		rangeState operator[](const int& num) const;

		char sname() const;
		std::string lname() const;
		std::string fname() const;

		friend class Parser;
	};
}