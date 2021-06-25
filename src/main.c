#include "async_serv.h"
#include "cryptocoins.h"
#include "json_rpc.h"
#include "signal_handler.h"
#include <pthread.h>
#include <time.h>

extern int start_lua(void);
// void __start(void){} #
extern void example(void);

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
  pthread_t pthreadServ;
  struct serv_arguments args = {argv[1], atoi(argv[2]), cryptocoins};
  void *(*fun)(void *) = (void *)serv_thread;
  if (pthread_create(&pthreadServ, NULL, fun, (void *)&args) != 0) {
    return eprintf("can't start thread!\n");
  } else
    puts("server is inited");
  start_lua();
  pthread_join(pthreadServ, NULL);

  clear_cryptocoins(cryptocoins);
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
