#include "libjson.h"

#define cleardata                                                              \
  free(s.ptr);                                                                 \
  curl_slist_free_all(headers);                                                \
  curl_easy_cleanup(curl);

int lua_rpc_requst(
    lua_State *L) { // struct cryptocoin * c, struct bitcoin_rpc_data * bdata)
  const char *data = lua_tostring(L, 2); // luaL_checkstring(L, 2);
  if (!lua_isstring(L, -1)) {
    luaL_error(L, "need data json(decoded) requst");
  }
  lua_pop(L, 1);

  if (!lua_istable(L, 1)) {
    luaL_error(L, "`table` error");
    return 0;
  }
#define GETLUASTRING(what)                                                     \
  lua_pushstring(L, "" #what);                                                 \
  lua_gettable(L, -2);                                                         \
  if (!lua_isstring(L, -1)) {                                                  \
    luaL_error(L, "invalid " #what);                                           \
  }                                                                            \
  char *rpc_##what = (char *)lua_tostring(L, -1);                              \
  lua_pop(L, 1);

  lua_pushstring(L, "port");
  lua_gettable(L, -2);
  if (!lua_isnumber(L, -1)) {
    luaL_error(L, "invalid rpcport");
  }
  uint rpc_port = (int)lua_tonumber(L, -1);
  lua_pop(L, 1);

  lua_pushstring(L, "istestnet");
  lua_gettable(L, -2);
  if (!lua_isboolean(L, -1)) {
    luaL_error(L, "invalid istestnet");
  }
  bool rpc_istestnet = (bool)lua_toboolean(L, -1);
  lua_pop(L, 1);

  GETLUASTRING(host);
  GETLUASTRING(user);
  GETLUASTRING(password);
  GETLUASTRING(name);
#undef GETLUASTRING

  struct cryptocoin a = {rpc_istestnet, rpc_user, rpc_password,
                         rpc_port,      rpc_host, rpc_name};
  CURL *curl = curl_easy_init();
  struct curl_slist *headers = NULL;

  if (curl) {
    char userpwd[strlen(a.rpcuser) + strlen(a.rpcpassword) + 2];
    char url[strlen(a.rpchost) + sizeof("http://") + sizeof("65535") + 2];
    sprintf(userpwd, "%s:%s%c", a.rpcuser, a.rpcpassword, '\0');
    sprintf(url, "http://%s:%d%c", a.rpchost, a.rpcport, '\0');

    if (s.ptr != NULL) {
#ifdef debug
      puts("clear old request");
#endif
      free(s.ptr);
      s.ptr = NULL;
      s.len = 0;
    }
    init_string(&s);

    // const char *data="";//bdata->json_ret;
    headers = curl_slist_append(headers, "content-type: text/plain;");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_URL, url);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(data));
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);

    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_TRY);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    curl_easy_perform(curl);

    lua_pushstring(L, s.ptr);
    lua_settop(L, -1);
  }
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
#undef cleardata
  return 1;
}

int luaopen_rpc(lua_State *L) {
  luaL_openlib(L, "rpc", rpclib, 0);
  return 1;
}
