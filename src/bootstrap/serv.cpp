#include "serv.h"
#include<iostream>
#include"opcodes.h"
#define doExit(...)                                                            \
  {                                                                            \
    eprintf(__VA_ARGS__);                                                      \
    exit(EXIT_FAILURE);                                                        \
  }
static opcode::opcode opcodes[] =
{
	{{0x01, opcode::ignorebyte, opcode::ignorebyte, 0x01},
                    opcode::event0},
	{{'a', 'b', 'c', 'd'},
                    opcode::event1},
};
constexpr auto serv_status_lua_var = "server_inited";
static bool serv_inited;
void serv_thread(const char * host, const uint16_t port, lua_State * L) {

  int ret;

  struct epoll_event ev, events[MAX_LISTEN];
  int epollfd, nfds;

  int main_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (main_descriptor == -1)
    doExit("Can't init socket\n");

  struct sockaddr_in my_addr;
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(port);
  my_addr.sin_addr.s_addr = inet_addr(host);

  ret = bind(main_descriptor, (struct sockaddr *)&my_addr,
             sizeof(struct sockaddr_in));
  if (ret == -1) {
    doExit("Can't init listen on %s:%d\n", host,
           port);
  }
  if (listen(main_descriptor, MAX_LISTEN) == -1)
    doExit("Cant start listening\n");
  printf("(serv) %s:%d listening\n", host, port);
  lua::pushval(L, true);
  lua_setglobal(L, serv_status_lua_var);
  lua_pop(L, 1);
  serv_inited = true;
  


  /*
          epollfd
  */
  epollfd = epoll_create1(0);
  if (epollfd == -1) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }
  ev.events = EPOLLIN;
  ev.data.fd = main_descriptor;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, main_descriptor, &ev) == -1) {
    perror("epoll_ctl: main_descriptor");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in client_addr;
  socklen_t sizeOfSockAddrType = sizeof(struct sockaddr_in);

  for (;;) { // for start
    // puts("Cycle");
    nfds = epoll_wait(epollfd, events, MAX_LISTEN, -1);
    if (nfds == -1) {
      puts("Epoll_Wait");
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }

    for (int n = 0; n < nfds; ++n) {
      if (events[n].data.fd == main_descriptor) {
        int conn_sock = accept(main_descriptor, (struct sockaddr *)&client_addr,
                               &sizeOfSockAddrType);
        if (conn_sock == -1) {
          perror("accept");
          puts("accept");
          exit(EXIT_FAILURE);
        }
        int flags = fcntl(conn_sock, F_GETFL, 0);
        fcntl(conn_sock, F_SETFL, flags | O_NONBLOCK);

        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = conn_sock;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
          puts("epoll_ctl");
          perror("epoll_ctl: conn_sock");
          exit(EXIT_FAILURE);
        }
      } /*if acceptor*/ else { /*if client*/
        char buf[1024];
        size_t nbytes;
        const char *ip = inet_ntoa(client_addr.sin_addr);
        const uint16_t port = htons(client_addr.sin_port);
        bzero(buf, sizeof(buf));
        if ((nbytes = recv(events[n].data.fd, buf, sizeof(buf), 0)) <= 0) {
          if (errno != EWOULDBLOCK && nbytes != 0) {
            perror("read/fd2");
          }
          epoll_ctl(epollfd, EPOLL_CTL_DEL, events[n].data.fd, &ev);
          close(events[n].data.fd);
          continue;
        }
		  lua_getglobal(L,serv_status_lua_var);
		  serv_inited = lua_toboolean(L,-1);	
		  lua_pop(L,1);

		if(!serv_inited){
			puts("Stop server?");
			return;
		}

/*
		   * 
  opcode::opcode op1{{0x02, opcode::ignorebyte, opcode::ignorebyte, 0x01},
                     NULL};
  opcode::opcode op2{{0x01, opcode::ignorebyte, opcode::ignorebyte, 0x01},
                     NULL};
  std::cout << (op == op2) << std::endl;
  */
		opcode::opcode_data data = {buf[0], buf[1], buf[2], buf[3]};
		for( auto op : opcodes ){
			if( op == data ){
				puts("Opcode found");
				op.getEvent().run(L, events[n].data.fd,
				                  ip, port, buf);
				break;
			}
		}//for
        // do_use_fd(events[n].data.fd);
      }//for(?)
    }  
  }     
}
