#pragma once
#ifndef luacxx
#define luacxx
#include"lua.h"

namespace lua{
	template<typename ...args>
	void pushval(lua_State * L, std::string s, args ... values);
	template<typename ...args>
	void pushval(lua_State * L, bool arg, args ... values);
	template<typename ...args>
	void pushval(lua_State * L, double arg, args ... values);
	template<typename ...args>
	void pushval(lua_State * L, int arg, args ... values);
	template<typename ...args>
	void pushval(lua_State * L, void* arg, args ... values);
	void pushval(lua_State *L){}
	namespace tables{
	/*example in C style - libbacteria code, libbacteria/lua/libimages.c and another parts*/
		void pushval(lua_State *L, std::map<std::string, int> m);
		void pushval(lua_State *L, std::map<std::string, std::string> m);
		void pushval(lua_State *L, std::map<std::string, const char*> m);
		void init(lua_State *L) ;

	/* ~example in C style - https://gist.github.com/SONIC3D/10388137 */
		void init(lua_State *L, int count_arrays, int count_reccords = 0);
		void pushval(lua_State *L, int n);
		void setfield(lua_State *L, std::string field, int s_pop = -2);
		void settable(lua_State *L, int s_pop = -3);
	};
}

namespace lua{
	template<typename ...args>
	void pushval(lua_State * L, const char * arg, args ... values){
		lua_pushstring(L,arg);
		pushval(L, values...);
		
	}
	template<typename ...args>
	void pushval(lua_State * L, std::string s, args ... values){
		lua_pushlstring(L, s.c_str(), s.size());
		pushval(L, values...);
	}

	template<typename ...args>
	void pushval(lua_State * L, bool arg, args ... values){
		lua_pushboolean(L,arg);
		pushval(L, values...);
	}
	template<typename ...args>
	void pushval(lua_State * L, double arg, args ... values){
		lua_pushnumber(L, arg);
		pushval(L, values...);
	}
	template<typename ...args>
	void pushval(lua_State * L, int arg, args ... values){
		lua_pushnumber(L, arg);
		pushval(L, values...);
	}
	template<typename ...args>
	void pushval(lua_State * L, void* arg, args ... values){
		if(arg == nullptr) 
			lua_pushnil(L);
		else
			lua_pushlightuserdata(L, (void*)arg);
		pushval(L, values...);
	}


//
	namespace tables{
	/*example in C style - libbacteria code, libbacteria/lua/libimages.c and another parts*/
		void pushval(lua_State *L, std::map<std::string, int> m){
			for(const auto el : m){
				LUA_PUSHTABLENUMBER(L, el.first.c_str(), el.second);
			}
		}
		void pushval(lua_State *L, std::map<std::string, std::string> m){
			for(const auto el : m)
				LUA_PUSHTABLESTRING(L, el.first.c_str(), el.second.c_str());
		}
		void pushval(lua_State *L, std::map<std::string, const char*> m){
			for(const auto el : m)
				LUA_PUSHTABLESTRING(L, el.first.c_str(), el.second);
		}
		void init(lua_State *L) { lua_newtable(L); }

	/* ~example in C style - https://gist.github.com/SONIC3D/10388137 */
		void init(lua_State *L, int count_arrays, int count_reccords) { 
			lua_createtable(L, count_arrays, count_reccords);
		}

		void pushval(lua_State *L, int n){
			lua_pushnumber(L, n);
		}
		void setfield(lua_State *L, std::string field, int s_pop){
			    lua_setfield(L, s_pop, field.c_str());
		}
		void settable(lua_State *L, int s_pop){
			lua_settable(L, s_pop);
		}
		
	};

};

#endif
