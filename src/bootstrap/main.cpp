
#include <iostream>
#include <thread>

#include "libbacteria/macros.h"
//#include"serv.h"
#include "lua/luaserv.h"
#include <getopt.h>
#include <unistd.h>
extern void serv_thread(const char *host, const uint16_t port, lua_State *L);

static void usage(char *const program) {
  puts(PROGRAM_INFO);
  auto printoption = [&program](const char *longopt, char opt = ' ',
                                const char *desc) {
    printf("--%s -%c\t=> %s\n", longopt, opt, desc);
  };
  printf("\nUSAGE: %s [options]\n", program);

  printoption("help", 'h', "help menu");
  printoption("listen host", 'l', "Host for listening(def: 127.0.0.1)");
  printoption("port", 'p', "Port for listening (def: 3245)");

  printoption("luahost", 'j', "Host of listeningluaserv (def: localhost)");
  printoption("luaport", 'k', "port of listeningluaserv (def: 6596)");
};

int main(int argc, char **argv) {
  char c;

  std::string host{"127.0.0.1"};
  uint16_t port = 3245;

  std::string luahost{"127.0.0.1"};
  uint16_t luaport = 6596;

  int option_index;
  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {"host", required_argument, 0, 'l'},
                                         {"port", required_argument, 0, 'p'},
                                         {"luahost", required_argument, 0, 'j'},
                                         {"luaport", required_argument, 0, 'k'},
                                         {0, 0, 0, 0}};
  while ((c = getopt_long(argc, argv, "hp:h:", long_options, &option_index)) !=
         -1) {
    switch (c) {
    case 0:
      if (std::string(long_options[option_index].name) ==
          std::string("usage")) {
        usage(argv[0]);
        exit(1);
      case 'j':
        luahost = optarg;
        break;
      case 'k':
        luaport = atoi(optarg);
        break;
      case 'l':
        host = optarg;
        break;
      case 'p':
        port = atoi(optarg);
        break;
      case '?':
        std::cerr << "Undefined argument" << std::endl;
      default:
        usage(argv[0]);
        exit(1);
        break;
      }
    }
  }
#ifndef DISABLELUA
  lua_State *L = start_lua();
  servArgs luasarg = {(char *)luahost.c_str(), luaport, L};

  // lua::pushval(L, true, 1, 2, "string", "fs", true, false, 1.0, 3.14);
  std::thread serv(serv_thread, host.c_str(), port, L);

  std::thread luas(luaServer, &luasarg);
  serv.join();
  lua_close(L);
#endif
  return 0;
}
