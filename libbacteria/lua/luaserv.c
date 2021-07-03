#include "lua/luaserv.h"
#ifdef __cplusplus
extern "C" {
#endif

void checkRetVal(lua_State *L, int n) {
  lua_getglobal(L, "checkRetVal");
  lua_pushnumber(L, n);
  if (lua_pcall(L, /*args*/ 1, /*results*/ 1, 0) != 0)
    luaL_error(L, "error running function `checkRetVal': %s",
               lua_tostring(L, -1));
  if (!lua_isnumber(L, -1)) {
    printf("(check retval[lua]) Ret val: %s\n", lua_tostring(L, -1));
  } else
    printf("(check retval[lua]) Ret val (num): %d\n", (int)lua_tonumber(L, -1));
  lua_pop(L, 1);
}

int luaServInit(servArgs *args) {
  if (strcmp(args->host, "127.0.0.1") != 0)
    fprintf(stderr, "WARNING: lua listening not in localhost!\n");
  int sock = socket(AF_INET, SOCK_STREAM, LUASOCK);
  if (sock == -1) {
    fprintf(stderr, "can't init socket for luaServer\n");
    return 0;
  }
  struct sockaddr_in luaServAddr;
  luaServAddr.sin_family = AF_INET;
  luaServAddr.sin_port = htons(args->port);
  luaServAddr.sin_addr.s_addr = inet_addr(args->host);
  int ret =
      bind(sock, (struct sockaddr *)&luaServAddr, sizeof(struct sockaddr_in));
  if (ret == -1) {
    close(sock);
    fprintf(stderr, "can't bind %s:%d\n", args->host, args->port);
    return 0;
  }
  if (listen(sock, LUAMAXLISTEN) == -1) {
    close(sock);
    fprintf(stderr, "cant listen %d users for lua server\n");
    return 0;
  }
  return sock;
}
// to settings file
void luaServer(servArgs *args) {

  int serv_sock = luaServInit(args);

  if (serv_sock == 0)
    return;
  int error; // lua error
  int csocket;
  size_t err_count, msg_size;
  struct sockaddr_in clientAddr;
  socklen_t addr_len = sizeof(struct sockaddr_in);
  printf("(lua server) listening on %s:%d\n", args->host, args->port);
  //	puts("dupping stdout");
  // int stdout_oldflags = fcntl(stdout_, F_GETFL, 0);
  // fcntl(stdout_, F_SETFL, stdout_oldflags | O_NONBLOCK);

  while ((csocket =
              accept(serv_sock, (struct sockaddr *)&clientAddr, &addr_len))) {
    int stdout_ = dup(STDOUT_FILENO);

    if (csocket == -1) {
      fprintf(stderr, "cant accept\n");
      err_count++;
    } else if (err_count)
      err_count--;
    if (err_count > MAXERRACCEPT)
      break;
    const char *ip = inet_ntoa(clientAddr.sin_addr);
    const uint16_t port = htons(clientAddr.sin_port);
    printf("(lua_server) Connected %s:%d\n", ip, port);
    char buf[LUABUF];
    bzero(buf, LUABUF);
    FILE *tmp, *tmp1;
#define LUATMPFILESTDOUT "./tmp_bacteria_lua.txt"
    while ((msg_size = recv(csocket, buf, LUABUF - 1, 0)) >
           0) {                                     // maybe can be -1(?)
      tmp = freopen(LUATMPFILESTDOUT, "w", stdout); // for lua_server
      tmp1 = fopen(LUATMPFILESTDOUT, "rb");
      error = luaL_loadbuffer(args->L, buf, strlen(buf), "line") ||
              lua_pcall(args->L, 0, 0, 0);
      if (error) {
        sprintf(buf, "%s", lua_tostring(args->L, -1));
        lua_pop(args->L, 1); /* pop error message from the stack */
        send(csocket, buf, strlen(buf), 0);
        fclose(tmp);
        fclose(tmp1);
      } else {
        // puts("Read stdout");
        fflush(stdout);
        //
        fclose(tmp);
        bzero(buf, LUABUF);
        (msg_size = fread(buf, LUABUF, 1, tmp1));
        send(csocket, buf, strlen(buf), 0);
        // send(csocket, buf, strlen(buf), 0);
        fclose(tmp1);
      }
      bzero(buf, LUABUF);
      tmp = fopen(LUATMPFILESTDOUT, "wb");
      fwrite("", 1, 1, tmp); // clear buffer.
      fclose(tmp);
    }
    // close(stdout_);
    fflush(stdout);
    fclose(stdout);
    stdout = fdopen(stdout_, "w");
    printf("(lua_server) lost connection %s:%d\n", ip, port); // to macros
    close(csocket);
  }
}

#ifdef __cplusplus
}
#endif
