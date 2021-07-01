#pragma once
#ifdef __cplusplus
	extern "C" {
#endif
#define luaL_reg      luaL_Reg
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include<dirent.h>

//typedef lua_State *L ...;
#define INITLUAFUNC(name) int lua_##name (lua_State *L)
#define LUAPAIR(name) {"" #name , lua_##name },

#define LUA_PUSHTABLESTRING(L,key,value){\
  lua_pushstring(L, key);\
  lua_pushstring(L, value);\
  lua_settable(L, -3);\
}

#define LUA_PUSHTABLENUMBER(L,key,value){\
  lua_pushstring(L, key);\
  lua_pushnumber(L, value);\
  lua_settable(L, -3);\
}

void runAllLuaFilesInDir(lua_State * L, const char * pathdir);
lua_State *  start_lua(void) ;

#ifdef __cplusplus
	}
#endif

