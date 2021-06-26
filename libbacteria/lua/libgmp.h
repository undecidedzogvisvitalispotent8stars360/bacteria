#pragma once
#include"lua.h"
#include<gmp.h>

	INITLUAFUNC(mpz_init);
	INITLUAFUNC(mpz_init_str);
	INITLUAFUNC(mpz_getstr);
	INITLUAFUNC(mpz_add);
	INITLUAFUNC(mpz_div);
	INITLUAFUNC(mpz_mul);
	INITLUAFUNC(mpz_mul_ui);
	INITLUAFUNC(mpz_sub);
	INITLUAFUNC(mpz_sub_ui);
	INITLUAFUNC(mpz_add_ui);
	INITLUAFUNC(mpz_clear);

	INITLUAFUNC(mpf_init);
	INITLUAFUNC(mpf_add);
	INITLUAFUNC(mpf_sub);
	INITLUAFUNC(mpf_sub_ui);
	INITLUAFUNC(mpf_div);
	INITLUAFUNC(mpf_mul);
	INITLUAFUNC(mpf_mul_ui);
	INITLUAFUNC(mpf_add_ui);
	INITLUAFUNC(mpf_getstr);
	INITLUAFUNC(mpf_init_str);
	INITLUAFUNC(mpf_clear);

	INITLUAFUNC(mpq_init);
	INITLUAFUNC(mpq_add);
//	INITLUAFUNC(mpq_sub);
//	INITLUAFUNC(mpq_sub_ui);
	INITLUAFUNC(mpq_add_ui);
	INITLUAFUNC(mpq_getstr);
	INITLUAFUNC(mpq_init_str);
	INITLUAFUNC(mpq_clear);




static const struct luaL_reg lgmp [] = {
	LUAPAIR(mpz_init)
	LUAPAIR(mpz_init_str)
	LUAPAIR(mpz_getstr)
	LUAPAIR(mpz_add)
	LUAPAIR(mpz_mul_ui)
	LUAPAIR(mpz_sub)
	LUAPAIR(mpz_sub_ui)
	LUAPAIR(mpz_div)
	LUAPAIR(mpz_mul)
	LUAPAIR(mpz_add_ui)
	LUAPAIR(mpz_clear)

	LUAPAIR(mpf_init)
	LUAPAIR(mpf_add)
	LUAPAIR(mpf_sub)
	LUAPAIR(mpf_sub_ui)
	LUAPAIR(mpf_mul_ui)
	LUAPAIR(mpf_div)
	LUAPAIR(mpf_mul)
	LUAPAIR(mpf_add_ui)
	LUAPAIR(mpf_getstr)
	LUAPAIR(mpf_init_str)
	LUAPAIR(mpf_clear)

	LUAPAIR(mpq_init)
	LUAPAIR(mpq_add)
	//LUAPAIR(mpq_sub)
	//LUAPAIR(mpq_div)
	LUAPAIR(mpq_add_ui)
	LUAPAIR(mpq_getstr)
	LUAPAIR(mpq_init_str)
	LUAPAIR(mpq_clear)
	{NULL,NULL}
};

int luaopen_liblgmp(lua_State *L);
