#include"libgmp.h"
#include<stdlib.h>
//static void gmpZToBuf(mpz_t v, char * buf, size_t bufSize){
//	gmp_snprintf(buf, bufSize, "%Zd", v);
//}

int luaopen_liblgmp(lua_State *L){
  luaL_openlib(L, "lgmp", lgmp, 0);
  return 1;
}

/*Integer*/
#define GETMP(prefix,name,where)\
  mp##prefix * l##name = (mp##prefix *)lua_touserdata(L, where);

INITLUAFUNC(mpz_init){
	mpz_t * rt = malloc(sizeof(mpz_t) * 1);
	mpz_init(*rt);
	lua_pushlightuserdata(L, rt);
	return 1;
}

INITLUAFUNC(mpz_init_str){
	mpz_t * rt = malloc(sizeof(mpz_t) * 1);
	const char * val = (const char*)luaL_checkstring(L, 2);
	mpz_init_set_str(*rt,val,0);
	lua_pushlightuserdata(L, rt);
	return 1;
}

INITLUAFUNC(mpz_add){
	GETMP(z_t, a,1);
	GETMP(z_t, b,2);
	GETMP(z_t, c,3);
	mpz_add(*la,*lb,*lc);
	return 0;
}

INITLUAFUNC(mpz_add_ui){
	GETMP(z_t, a,1);
	GETMP(z_t, b,2);
	int c = luaL_checknumber(L,3);
	mpz_add_ui(*la,*lb,c);
	return 0;
}

INITLUAFUNC(mpz_getstr){
	GETMP(z_t, a,1);
	int c = luaL_checknumber(L,2);
	char buf[c];
	gmp_snprintf(buf,c, "%Zd", *la);
	lua_pushstring(L,buf);
	return 1;
}

INITLUAFUNC(mpz_clear){
	GETMP(z_t, a,1);
	if( *la == NULL )luaL_error(L,"Invalid mpz pointer");
	mpz_clear(*la);
	free(la);
	return 0;
}

/*Float*/
INITLUAFUNC(mpf_init){

}

INITLUAFUNC(mpf_add){

}

INITLUAFUNC(mpf_add_ui){

}

INITLUAFUNC(mpf_getstr){

}

INITLUAFUNC(mpf_init_str){

}

INITLUAFUNC(mpf_clear){

}


/*Rational*/

INITLUAFUNC(mpq_init){

}

INITLUAFUNC(mpq_add){

}

INITLUAFUNC(mpq_add_ui){

}
INITLUAFUNC(mpq_init_str){

}

INITLUAFUNC(mpq_getstr){

}

INITLUAFUNC(mpq_clear){

}
