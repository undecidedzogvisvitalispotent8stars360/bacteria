#pragma once
#ifndef EVENTS_H
#define EVENTS_H
#include"lua/lua.h"
#include<stdint.h>
#include<array>

namespace events{
//is idea to disable lua for opcodes as wrapper if is need.
#define EVENTDEF lua_State * L, int sock, const char * uIp, uint16_t uPort, char* buf
	using fun =  void(*)(EVENTDEF, ...);

	class event{
			protected:
				fun m_fun;
			public:
				event(fun f): m_fun(f)
				{}
template<typename ...args>
void run(EVENTDEF, args ... values){ 
	m_fun(L, sock, (const char*)uIp, uPort, buf, values...); 
};

	};

};
#endif
