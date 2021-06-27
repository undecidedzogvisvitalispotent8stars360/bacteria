#include "async_serv.h"
#include "cryptocoins.h"
#include "json_rpc.h"
#include "signal_handler.h"
#include <pthread.h>
#include <time.h>

//extern int start_lua(void);
// void __start(void){} #
extern void example(void);

#define LUASOCK 0
#define LUAMAXLISTEN 3

typedef struct {
	char * host; unsigned int port;
	lua_State * L;
} servArgs;

static int luaServInit(servArgs * args){
	if( strcmp(args->host, "127.0.0.1") != 0) fprintf(stderr,"WARNING: lua listening not in localhost!\n");
	int sock=socket(AF_INET, SOCK_STREAM, LUASOCK);
	if(sock == -1){
		fprintf(stderr,"can't init socket for luaServer\n");
		return 0;
	} 
	
        struct sockaddr_in luaServAddr;
        luaServAddr.sin_family = AF_INET;
        luaServAddr.sin_port = htons(args->port);
        luaServAddr.sin_addr.s_addr = inet_addr(args->host);
        int ret = bind(sock, (struct sockaddr *)&luaServAddr,
             sizeof(struct sockaddr_in));
        if (ret == -1) {
		close(sock);
        	fprintf(stderr,"can't bind %s:%d\n", args->host, args->port);
		return 0;
        }
        if (listen(sock, LUAMAXLISTEN) == -1){
		close(sock);
		fprintf(stderr,"cant listen %d users for lua server\n");
		return 0;
	}
	return sock;
}
#define MAXERRACCEPT 5
#define LUABUF 2056

void luaServer(servArgs * args){


	int serv_sock = luaServInit(args);
 
	if(serv_sock == 0)
		return;
	int error;//lua error
	int csocket;
	size_t err_count, msg_size;
	struct sockaddr_in clientAddr;
	socklen_t addr_len =  sizeof(struct sockaddr_in);
	printf("(lua server) listening on %s:%d\n", args->host, args->port);
	while( (csocket = accept(serv_sock,(struct sockaddr*)&clientAddr,&addr_len)) ){
		//freopen("stdout.txt", "w", stdout); //for lua_server
		if(csocket == -1){
			fprintf(stderr,"cant accept\n");
			err_count++;
		}else if(err_count)err_count--;
		if(err_count > MAXERRACCEPT) break;
	        const char *ip = inet_ntoa(clientAddr.sin_addr);
	        const uint16_t port = htons(clientAddr.sin_port);
		printf("(lua_server) Connected %s:%d\n", ip, port);
		char buf[LUABUF];
		bzero(buf, LUABUF);
		while( (msg_size = recv(csocket, buf, LUABUF-1, 0)) > 0 ){ //maybe can be -1(?)
			    error =
			        luaL_loadbuffer(args->L, buf, strlen(buf), "line") || lua_pcall(args->L, 0, 0, 0);
			    if (error) {
			      fprintf(stderr, "%s", lua_tostring(args->L, -1));
			      lua_pop(args->L, 1); /* pop error message from the stack */
			    }
			    bzero(buf, LUABUF);
		}
		printf("(lua_server) lost connection %s:%d\n", ip, port); // to macros
		close(csocket);
	}
}

int main(int argc, char **argv) {
  
  catch_badsignals();
  srand(time(NULL));
  if (argc != 3)
    return eprintf("%s host port\n", argv[0]);

  struct cryptocoin *cryptocoins = init_cryptocoins("cryptocoins.ini");
  // dump_cryptocoins(cryptocoins);
  example();

  /*
          server part
  */
  puts("start server");
  pthread_t pthreadServ, pthreadLuaServ;
  struct serv_arguments args = {argv[1], atoi(argv[2]), cryptocoins};
  void *(*mainserv_fun)(void *) = (void *)serv_thread;
  void *(*luaserv_fun)(void *) = (void *)luaServer;

  if (pthread_create(&pthreadServ, NULL, mainserv_fun, (void *)&args) != 0) {
    return eprintf("can't start thread!\n");
  } else
    puts("main server is inited");
  lua_State * L = start_lua();
  puts("start lua server");


  servArgs args_luaserv = {"127.0.0.1", 6565, L};

  if (pthread_create(&pthreadLuaServ, NULL, luaserv_fun, (void *)&args_luaserv) != 0) {
    return eprintf("can't start thread!\n");
  } else
    puts("lua serv inited");

  pthread_join(pthreadLuaServ, NULL);
  pthread_join(pthreadServ, NULL);
  
  

  clear_cryptocoins(cryptocoins);
  lua_close(L);
}

void example(void) {
  struct bitcoin_rpc_data bdata = {"listaccounts", brpc_prepare_params(NULL)};
  brpc_get_json(&bdata);
  const char *account_name = "ANCMS_Abj1pcMsse";
  struct cryptocoin c = {
      false, "gostcoinrpc", "97WDPgQADfazR6pQRdMEjQeDeCSzTwVaMEZU1dGaTmLo",
      19376, "127.0.0.1",   NULL};
  json_t *data = brpc_json_request(&c, &bdata);
  json_t *result = json_object_get(data, "result");

  if (!json_is_object(result)) {
    puts("Result not found");
    json_decref(data);
    //    	exit(1);
  } else {
    json_t *ancms_account = json_object_get(result, account_name);
    float balance = json_number_value(ancms_account);
    printf("Account balance of %s: %f\n", account_name, balance);
  }
  json_decref(data);
}
