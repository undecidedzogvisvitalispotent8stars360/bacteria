#ifdef __cplusplus
extern "C"{
#endif

#include"lua/lua.h"
#include<sys/types.h>          /* See NOTES */
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<unistd.h>
#include<time.h>
#include<string.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<unistd.h>

#define LUASOCK 0
#define LUAMAXLISTEN 3
typedef struct {
  char *host;
  unsigned int port;
  lua_State *L;
} servArgs;
#define MAXERRACCEPT 5
#ifndef NOTMUCHBUF
#define LUABUF 5012 * 2
#else
#ifdef VERYSMALLBUF
#define LUABUF 128
#else
#define LUABUF 512
#endif
#endif


/*static*/ void checkRetVal(lua_State *L, int n);
int luaServInit(servArgs *args);
void luaServer(servArgs *args);

#ifdef __cplusplus 
}
#endif

