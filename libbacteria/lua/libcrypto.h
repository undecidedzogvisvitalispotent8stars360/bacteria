#pragma once
#include "lua.h"
#include"cryptocoins.h"
static bool cryptocoinsinited=false;

//static int example (lua_State *L) { // not need
//
//	struct bitcoin_rpc_data bdata = {"listaccounts", brpc_prepare_params(NULL)};
//	brpc_get_json(&bdata);
//	const char * account_name = "ANCMS_Abj1pcMsse";
//	struct cryptocoin  c = {false, "gostcoinrpc", "97WDPgQADfazR6pQRdMEjQeDeCSzTwVaMEZU1dGaTmLo",
//		 19376, "127.0.0.1", NULL};
//	json_t * data = brpc_json_request(&c,&bdata);
//	json_t * result = json_object_get(data, "result");
//
//	if(!json_is_object(result))
//    	{
//		puts("Result not found");
//        	json_decref(data);
//    //    	exit(1);
//    	}else{
//		   json_t * ancms_account = json_object_get(result, account_name);
//		   float balance = json_number_value(ancms_account);
//		   printf("Account balance of %s: %f\n",account_name,balance);
//	}
//        json_decref(data);
//    
//}

int loadCryptocoins (lua_State *L);
int getCryptocoinsTable(lua_State* L);
int luaclear_cryptocoins(lua_State * L);

void lua_loadscript (lua_State * L, const char * filename);
void luagetcount_cryptocoins(lua_State * L, unsigned int * c);

static const struct luaL_reg cryptocoinslib [] = {
   //   {"example", example},
      {"load", loadCryptocoins},
      {"gettable", getCryptocoinsTable},
      {"clear",luaclear_cryptocoins},
      //{"size", luagetcount_cryptocoins},
      {NULL, NULL}
};
int luaopen_cryptocoins (lua_State *L);


