#include "libcrypto.h"
#include "libed25519rsa.h"
#include "libencdec.h"
#include "libgmp.h"
#include "libimages.h"
#include "libjson.h"

void runAllLuaFilesInDir(lua_State *L, const char *path) {
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(path)) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
        continue;
      char tmp[strlen(path) + strlen(ent->d_name) + 2];
      sprintf(tmp, "%s/%s", path, ent->d_name);
      printf("init: %s\n", ent->d_name);
      // lua_loadscript(L,tmp);
      luaL_dofile(L, tmp);
    }
    closedir(dir);
  } else {
    fprintf(stderr, "WARNING: could not found %s submodule directrory\n", path);
  }
}

static const char init_lua_file_path[] = "./luasubmodules/init.lua";
/*extern int (*luaopen_luasql_postgres)(lua_State *L, lua_CFunction fn, int n);
static void luaopen_sql(lua_State * L){
        lua_getglobal(L, "package");
        lua_getfield(L, -1, "preload");
        lua_pushcfunction(L, luaopen_luasql_postgres);
        lua_setfield(L, -2, "luasql.postgres");
        lua_pop(L, 2);
}*/

lua_State *start_lua(void) {
  //  char buff[256];
  //  int error;
  lua_State *L = lua_open(); /* opens Lua */
  //      luaopen_base(L);             /* opens the basic library */
  //      luaopen_table(L);            /* opens the table library */
  //      luaopen_io(L);               /* opens the I/O library */
  //      luaopen_string(L);           /* opens the string lib. */
  //      luaopen_math(L);             /* opens the math lib. */
  // luaopen_dir(L);
  luaL_openlibs(L);
  luaopen_cryptocoins(L);
  luaopen_rpc(L);
  luaopen_encdec(L);
  luaopen_libimages(L);
  luaopen_liblgmp(L);
  luaopen_libed25519rsa(L);
  // luaopen_sql(L);
  // initLuaSubmodules(L, "./luasubmodules");
  lua_loadscript(L, init_lua_file_path);
  return L;
  //  while (fgets(buff, sizeof(buff), stdin) != NULL) {
  //    error =
  //        luaL_loadbuffer(L, buff, strlen(buff), "line") || lua_pcall(L, 0, 0,
  //        0);
  //    if (error) {
  //      fprintf(stderr, "%s", lua_tostring(L, -1));
  //      lua_pop(L, 1); /* pop error message from the stack */
  //    }
  //  }

  //  lua_close(L);
  //  return 0;
}
