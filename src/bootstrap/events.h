#pragma once
#ifndef EVENTS_H
#define EVENTS_H
#include"lua/luacxx.h"
namespace events{
//is idea to disable lua for opcodes as wrapper if is need.
#ifndef DISABLELUA
	using fun =  void(*)(lua_State * L, ...);
#else
	using fun =  void(*)(const char params[], ...);
#endif
	class event{
			protected:
				fun m_fun;
			public:
				event(fun f): m_fun(f)
				{}
template<typename ...args>
#ifndef DISABLELUA
		void run(lua_State * L,const char params[], args ... values){ m_fun(L,params, values...); };
#else
		void run(const char params[], args ... values){ m_fun(params, values...); };
#endif
	};

};
#endif
