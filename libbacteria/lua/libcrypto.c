#include "libcrypto.h"
extern const struct luaL_reg cryptocoinslib[];
//#define debug
int luaopen_cryptocoins(lua_State *L) {
  luaL_openlib(L, "cryptocoins", cryptocoinslib, 0);
  return 1;
}
int loadCryptocoins(lua_State *L) {
  if (cryptocoinsinited)
    luaL_error(L, "you already loaded cryptocoin config file!\n");
  const char *pathtofile = luaL_checkstring(L, 1);
  struct cryptocoin *crypto = init_cryptocoins(pathtofile);
  if (crypto == NULL) {
    luaL_error(L, "%s load failed\n", pathtofile);
  }
  lua_pushnumber(L, getCountCryptocoins());
  lua_setglobal(L, "count_cryptocoins");
#ifdef debug
  dump_cryptocoins(crypto);
#endif

  size_t nbytes =
      sizeof(struct cryptocoin) + (strlen(crypto->rpcuser) + 1) +
      (strlen(crypto->rpcpassword) + 1) + (strlen(crypto->rpchost) + 1) +
      (strlen(crypto->cryptocoin_name) + 1) + sizeof(bool) * (crypto->testnet) +
      sizeof(uint16_t) * (crypto->rpcport - 1);
  ;
  struct cryptocoin *a = (struct cryptocoin *)lua_newuserdata(L, nbytes);
#define copydata(a, crypto, what, pre)                                         \
  {                                                                            \
    a->pre##what = malloc(strlen(crypto->pre##what) * sizeof(char));           \
    strcpy(a->pre##what, crypto->pre##what);                                   \
  }

  memcpy(a, crypto, nbytes);
  /*copydata(a,crypto,user,rpc);
  copydata(a,crypto,password,rpc);
  copydata(a,crypto,host,rpc);
  copydata(a,crypto,coin_name,crypto);*/

#undef copydata
  cryptocoinsinited = true;
  return 1;
}

void luagetcount_cryptocoins(lua_State *L, unsigned int *c) {
  lua_getglobal(L, "count_cryptocoins");
  if (!lua_isnumber(L, -1))
    luaL_error(L, "`count_cryptocoins' should be a number\n");
  *c = (int)lua_tonumber(L, -1);
}

int luaclear_cryptocoins(lua_State *L) {
  struct cryptocoin *crypto = (struct cryptocoin *)lua_touserdata(L, 1);
  luaL_argcheck(L, crypto != NULL, 1,
                "`array' expected. t=cryptocoins.load(...pathtoIni) "
                "cryptocoins.clear(t)");
  for (unsigned char i = 0; crypto[i].rpchost != NULL &&
                            crypto[i].rpcport != 0 && i < count_cryptocoins;
       i++) {
    free(crypto->rpcuser);
    free(crypto->rpcpassword);
    free(crypto->rpchost);
    free(crypto->cryptocoin_name);
    setCountCryptocoins(0);
  }
  cryptocoinsinited = false;
}
/*
        bool testnet;
        char * rpcuser;
        char * rpcpassword;
        uint16_t rpcport;
        char * rpchost;
        char * cryptocoin_name;
*/

int getCryptocoinsTable(lua_State *L) {
  unsigned int c;
  luagetcount_cryptocoins(L, &c);
  lua_createtable(L, 0, 4);
  struct cryptocoin *crypto = (struct cryptocoin *)lua_touserdata(L, 1);
  luaL_argcheck(L, crypto != NULL, 1,
                "`array' expected. t=cryptocoins.load(...pathtoIni) "
                "cryptocoins.gettable(t)");

#ifdef debug
  dump_cryptocoins(crypto);
#endif

  const char *needcoin = luaL_checkstring(L, 2);
  bool found = false;
  // printf("count cryptocoins: %d\n", c );
  for (unsigned char i = c; i--;) {
    if (strcmp(needcoin, crypto[i].cryptocoin_name) == 0) {
      crypto = &(crypto[i]);
      found = true;
      break;
    }
  }
  if (!found)
    luaL_error(L, "%s cryptocoin not found\n", needcoin);

  //	lua_pushstring(L, crypto->cryptocoin_name);
  //	lua_createtable(L,0,0);

  lua_pushstring(L, "istestnet");
  lua_pushboolean(L, crypto->testnet ? 1 : 0);
  lua_settable(L, -3); /* 3rd element from the stack top */

  LUA_PUSHTABLESTRING(L, "user", crypto->rpcuser);
  LUA_PUSHTABLESTRING(L, "password", crypto->rpcpassword);
  LUA_PUSHTABLESTRING(L, "host", crypto->rpchost);
  LUA_PUSHTABLESTRING(L, "name", crypto->cryptocoin_name);

  lua_pushstring(L, "port");
  lua_pushnumber(L, crypto->rpcport);
  lua_settable(L, -3);
  return 1;
}

void lua_loadscript(lua_State *L, const char *filename) {
  /*
        lua_State *L = lua_open();
        luaopen_base(L);
        luaopen_io(L);
        luaopen_string(L);
        luaopen_math(L);
  */
  if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
    luaL_error(L, "cannot run configuration file: %s", lua_tostring(L, -1));
  /*
    lua_getglobal(L, "width");
    lua_getglobal(L, "height");
    if (!lua_isnumber(L, -2))
      error(L, "`width' should be a number\n");
    if (!lua_isnumber(L, -1))
      error(L, "`height' should be a number\n");
    *width = (int)lua_tonumber(L, -2);
    *height = (int)lua_tonumber(L, -1);
    lua_close(L);
*/
}
