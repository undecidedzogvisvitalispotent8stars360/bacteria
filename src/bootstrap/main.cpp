#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<array>
#include<vector>
#include<iostream>
#include<map>
//#include"lua/lua.h"
#include"lua/luacxx.h"


//extern void event0(const char params[], ...);
//extern void event1(const char params[],...);

namespace split{
//constexpr auto bufsize=2056;
char ** 
split_msg(char * buf, const char schar, size_t * splitted_size, size_t msg_len){
/*
*/
	const unsigned long long max_splitted = 120;
	size_t arr_size=0;
	char **splitted;
	splitted = (char**)malloc( sizeof(char*) * arr_size );
	if(!splitted) abort();
	char * str;
	char str2[msg_len];
	do{
		bzero(str2, msg_len);
		str = strchr(buf, schar);
	

		if(str != NULL || ( str = strchr(buf, '\r') )!=NULL ){
			arr_size++;
			splitted = (char**)realloc( splitted,sizeof(char*) * arr_size );
			if(!splitted) abort();

			memcpy(str2, buf, (str-buf));
			//printf("str2 = %s\n", str2);

			splitted[arr_size-1] = (char*)malloc( sizeof(char) * strlen(str2) + 1);
			if( !splitted[arr_size-1] ) abort();
			
			strcpy(splitted[arr_size-1], str2); 
			splitted[arr_size-1][strlen(str2)]=0;
		}
		buf=str+1;
	}while(str != NULL && arr_size < max_splitted);
	*splitted_size=arr_size;

	return splitted;
}
void 
free_splitted(char  ** what, size_t n){
	for(n;n--;){
		if(n == 0) break;
		if( what[n][0] == 0 ) continue;
		free((void*) (what[n-1]) );
	}
	free((void*)what);
}

std::vector<std::string> split(char * buf, const char schar, size_t bufsize=0){
	size_t split_size;
	std::vector<std::string> rt;
	if(bufsize == 0)
		bufsize = strlen(buf);
	char ** splitted = split_msg(buf, schar, &split_size, bufsize);
	for(size_t i = 0; i < split_size; i++){
		rt.push_back( splitted[i] );
	}
	free_splitted(splitted, split_size);
	return rt;
}


};
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

namespace opcode{

	void event1(lua_State * L, ...){
		puts("event1");
	}

	void event0(lua_State * L,  ...){
		puts("event0");
	}

	
	constexpr short opcode_size = 4;
	constexpr auto ignorebyte = 0x0E;
	constexpr auto splitbyte = 0x0F;
	using eventfun = events::fun;
	using opcode_data = std::array<char, opcode_size>;

class opcode{
	protected:

	protected:
		opcode_data m_data;
		events::event m_event;
	public:
		bool operator==(opcode& a) noexcept{
			for(unsigned short i = 0; i < opcode_size; i++){
				if(a.m_data[i] != m_data[i]) return false;
			}
			return true;
		}

		template<typename T>
		bool operator==(T const a) noexcept{
			for(unsigned short i = 0; i < opcode_size; i++){
				if(a[i] != m_data[i]) return false;
			}
			return true;
		}
		template<typename T>
		opcode& operator=(T a[]) noexcept{
			unsigned long i =0;
			for(auto b : a){
				m_data[i++]=b;
			}
			return *this;
		}
		opcode(opcode_data data, eventfun fun):
		m_data(data), m_event(fun)
		{}
		events::event& getEvent(void) { return m_event; }



};

}


int
main(void)
{

	#ifndef DISABLELUA
		lua_State * L = start_lua();
		lua::pushval(L, true,1, 2, "string", "fs", true, false, 1.0, 3.14 );

		lua_close(L);
	#endif
	opcode::opcode_data data1 = {0x01,opcode::ignorebyte,opcode::ignorebyte,0x01};

	opcode::opcode op{ {0x01, opcode::ignorebyte, opcode::ignorebyte, 0x01}, opcode::event0 };
	opcode::opcode op1{ {0x02, opcode::ignorebyte, opcode::ignorebyte, 0x01}, NULL };
	opcode::opcode op2{ {0x01, opcode::ignorebyte, opcode::ignorebyte, 0x01}, NULL };
	std::cout << (op == op2) << std::endl;
	std::cout << (op == data1) << std::endl;
	op.getEvent().run(NULL,"",NULL);


	return 0;
}
