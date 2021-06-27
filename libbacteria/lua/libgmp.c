#include "libgmp.h"
#include <stdlib.h>
// static void gmpZToBuf(mpz_t v, char * buf, size_t bufSize){
//	gmp_snprintf(buf, bufSize, "%Zd", v);
//}

int luaopen_liblgmp(lua_State *L) {
  luaL_openlib(L, "lgmp", lgmp, 0);
  return 1;
}

/*Integer*/
#define GETMP(prefix, name, where)                                             \
  mp##prefix *l##name = (mp##prefix *)lua_touserdata(L, where);                \
  if (l##name == NULL)                                                         \
    luaL_error(L, "l " #prefix " init err GMP");

INITLUAFUNC(mpz_div) {
  GETMP(z_t, a, 1);
  GETMP(z_t, b, 2);
  GETMP(z_t, c, 3);
  mpz_div(*la, *lb, *lc);
  return 0;
}

INITLUAFUNC(mpz_mod) {
  GETMP(z_t, a, 1);
  GETMP(z_t, b, 2);
  GETMP(z_t, c, 3);
  mpz_mod(*la, *lb, *lc);
  return 0;
}

INITLUAFUNC(mpz_mul) {
  GETMP(z_t, a, 1);
  GETMP(z_t, b, 2);
  GETMP(z_t, c, 3);
  mpz_mul(*la, *lb, *lc);
  return 0;
}

INITLUAFUNC(mpz_mul_ui) {
  GETMP(z_t, a, 1);
  GETMP(z_t, b, 2);
  int c = luaL_checknumber(L, 3);
  mpz_mul_ui(*la, *lb, c);
  return 0;
}

INITLUAFUNC(mpz_init) {
  mpz_t *rt = malloc(sizeof(mpz_t) * 1);
  mpz_init(*rt);
  lua_pushlightuserdata(L, rt);
  return 1;
}

INITLUAFUNC(mpz_init_str) {
  mpz_t *rt = malloc(sizeof(mpz_t) * 1);
  const char *val = (const char *)luaL_checkstring(L, 1);
  mpz_init_set_str(*rt, val, 0);
  lua_pushlightuserdata(L, rt);
  return 1;
}

INITLUAFUNC(mpz_add) {
  GETMP(z_t, a, 1);
  GETMP(z_t, b, 2);
  GETMP(z_t, c, 3);
  mpz_add(*la, *lb, *lc);
  return 0;
}

INITLUAFUNC(mpz_add_ui) {
  GETMP(z_t, a, 1);
  GETMP(z_t, b, 2);
  int c = luaL_checknumber(L, 3);
  mpz_add_ui(*la, *lb, c);
  return 0;
}

INITLUAFUNC(mpz_sub) {
  GETMP(z_t, a, 1);
  GETMP(z_t, b, 2);
  GETMP(z_t, c, 3);
  mpz_sub(*la, *lb, *lc);
  return 0;
}

INITLUAFUNC(mpz_sub_ui) {
  GETMP(z_t, a, 1);
  GETMP(z_t, b, 2);
  int c = luaL_checknumber(L, 3);
  mpz_sub_ui(*la, *lb, c);
  return 0;
}

#define LGMPGETSTR(prefix, format)                                             \
  GETMP(prefix, a, 1);                                                         \
  int c = luaL_checknumber(L, 2);                                              \
  char buf[c];                                                                 \
  gmp_snprintf(buf, c, format, *la);                                           \
  lua_pushstring(L, buf);                                                      \
  return 1;

INITLUAFUNC(mpz_getstr) { LGMPGETSTR(z_t, "%Zd"); }

INITLUAFUNC(mpz_clear) {
  GETMP(z_t, a, 1);
  if (*la == NULL)
    luaL_error(L, "Invalid mpz pointer");
  mpz_clear(*la);
  free(la);
  return 0;
}

/*Float*/
INITLUAFUNC(mpf_div) {
  GETMP(f_t, a, 1);
  GETMP(f_t, b, 2);
  GETMP(f_t, c, 3);
  mpf_div(*la, *lb, *lc);
  return 0;
}

INITLUAFUNC(mpf_mul) {
  GETMP(f_t, a, 1);
  GETMP(f_t, b, 2);
  GETMP(f_t, c, 3);
  mpf_mul(*la, *lb, *lc);
  return 0;
}

INITLUAFUNC(mpf_mul_ui) {
  GETMP(f_t, a, 1);
  GETMP(f_t, b, 2);
  int c = luaL_checknumber(L, 3);
  mpf_mul_ui(*la, *lb, c);
  return 0;
}

INITLUAFUNC(mpf_init) {
  mpf_t *rt = malloc(sizeof(mpf_t) * 1);
  mpf_init(*rt);
  lua_pushlightuserdata(L, rt);
  return 1;
}

INITLUAFUNC(mpf_init_str) {
  mpf_t *rt = malloc(sizeof(mpf_t) * 1);
  const char *val = (const char *)luaL_checkstring(L, 1);
  mpf_init_set_str(*rt, val, 0);
  lua_pushlightuserdata(L, rt);
  return 1;
}

INITLUAFUNC(mpf_add) {
  GETMP(f_t, a, 1);
  GETMP(f_t, b, 2);
  GETMP(f_t, c, 3);
  mpf_add(*la, *lb, *lc);
  return 0;
}

INITLUAFUNC(mpf_add_ui) {
  GETMP(f_t, a, 1);
  GETMP(f_t, b, 2);
  float c = luaL_checknumber(L, 3);
  printf("float val: %f\n", c);
  mpf_add_ui(*la, *lb, c);
}

INITLUAFUNC(mpf_sub) {
  GETMP(f_t, a, 1);
  GETMP(f_t, b, 2);
  GETMP(f_t, c, 3);
  mpf_sub(*la, *lb, *lc);
  return 0;
}

INITLUAFUNC(mpf_sub_ui) {
  GETMP(f_t, a, 1);
  GETMP(f_t, b, 2);
  int c = luaL_checknumber(L, 3);
  mpf_sub_ui(*la, *lb, c);
  return 0;
}

INITLUAFUNC(mpf_getstr) { LGMPGETSTR(f_t, "%.Ff"); }

INITLUAFUNC(mpf_clear) {
  GETMP(f_t, a, 1);
  mpf_clear(*la);
  free(la);
}

#undef LGMPGETSTR

/*Rational*/

INITLUAFUNC(mpq_init) {}

INITLUAFUNC(mpq_add) {}

INITLUAFUNC(mpq_add_ui) {}
INITLUAFUNC(mpq_init_str) {}

INITLUAFUNC(mpq_getstr) {}

INITLUAFUNC(mpq_clear) {}
