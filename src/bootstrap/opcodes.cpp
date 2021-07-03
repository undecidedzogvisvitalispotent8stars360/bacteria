// extern void event1(const char params[],...);
#include <cstddef>
#include <iostream>
#include"opcodes.h"

extern "C" {
	 #include <lua.h>
         #include <lualib.h>
    	 #include <lauxlib.h>
	 #include"lua/luaserv.h"
};

namespace opcode {

void event1(lua_State * L, int sock, const char * uIp, uint16_t uPort, char* buf, ...) {
 puts("event1 -"); //
  lua_getglobal(L, "event1");
  lua_pushnumber(L, sock);
  lua_pushstring(L, uIp);
  lua_pushnumber(L, uPort);
  lua_pushstring(L, (buf+4));

  if (lua_pcall(L, /*args*/ 4, /*results*/ 1, 0) != 0)
    luaL_error(L, "error running function `event1': %s",
               lua_tostring(L, -1));
  unsigned char retdata[512];
  if (!lua_isnumber(L, -1)) {
    printf("(check retval[lua]) Ret val: %s\n", lua_tostring(L, -1));
    sprintf((char*)retdata,"%s", lua_tostring(L,-1));
  } else{
    sprintf((char*)retdata,"%d", lua_tonumber(L,-1));
    printf("(check retval[lua]) Ret val (num): %d\n", (int)lua_tonumber(L, -1));
  }
  lua_pop(L, 1);

  if (send(sock, retdata, strlen((char*)retdata), MSG_NOSIGNAL) < 0) {
          perror("Write error");
  }


}

void event0(lua_State * L, int sock, const char * uIp, uint16_t uPort, char* buf, ...) { 
	puts("event0 - "); 
}

} // namespace opcode
