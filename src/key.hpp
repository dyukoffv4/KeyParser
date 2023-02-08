#pragma once

#include "defines.hpp"

namespace KP {
	class Key {
	private:
		char s_data;
		std::string l_data;
		int lk_num;
		int hk_num;

		static Key getNull(int f_num = 0, int s_num = 0);
		static Key getRoot(int f_num = -1, int s_num = -1);

	public:
		enum State{E, S, L, A};
		enum zoneState{ZS_L, ZS_I, ZS_H};

		explicit Key(const char& s_data, int f_num = -1, int s_num = -1);
		explicit Key(const std::string& l_data, int f_num = -1, int s_num = -1);
		explicit Key(const char& s_data, const std::string& l_data, int f_num = -1, int s_num = -1);

		State getState() const;

		bool operator<(const Key& key) const;
		bool operator==(const Key& key) const;
		zoneState operator[](const int& num) const;

		char sname() const;
		std::string lname() const;
		std::string fname() const;

		friend class Terminal;
	};
}