#pragma once

#include <map>
#include <iostream>
#include "key.hpp"

namespace KP {
	class Terminal {
	public:
		enum rootState{RS_F, RS_S, RS_A};

	private:
		rootState state;
		std::map<Key, void (*)(Args)> binds;
		void (*last)(void);

	public:
		Terminal(rootState state = RS_F);

		Terminal& operator=(const Terminal& term);

		void setKey(Key key, void (*lnr)(Args) = nullptr);
		void delKey(Key key);

		void setRootRange(int f_num, int s_num);
		void setRootState(rootState state);
		void setRoot(void (*lnr)(Args));
		void delRoot();

		void setFinal(void (*lnr)(void));
		void delFinal();

		void cleanBinds();
		
		void execute(int argc, char* argv[]);
		void execute(Args input);
	};
}