#include<gmp.h>
#include<stdio.h>


void mul(mpz_t result, const mpz_t param, unsigned long n){
	unsigned long i;
	mpz_mul_ui(result, param, n);
	for(i = 1; i<n;i++){
		mpz_add_ui(result,result, i*7);// unsigned long add to mpz_t;
		mpz_add(result,result,result);//mpz_t add to mpz_t
	}
}

int main(int cn, char**v){
  if(cn != 3) return fprintf(stderr,"%s a b\n",v[0]);
  mpf_t x, y, c;
  mpf_init (c);
  mpf_init (x);           /* use default precision */
  mpf_init2 (y, 256);     /* precision at least 256 bits */
  mpf_init_set_str(x, v[1], 0);
  mpf_init_set_str(y, v[2], 0);
  mpf_div(c, x,y);
 // mpf_out_str(stdout, 0, 65535, c);
  gmp_printf("%.Ff\n", c);
  puts("");
  /* Unless the program is about to exit, do ... */
  mpf_clear (x);
  mpf_clear (y);
  mpf_clear(c);

  //
  mpz_t r,n;
  mpz_init(r);
  mpz_init_set_str(n, "123321", 0);
  mul(r,n,/*65535*/ 1000);
  gmp_printf("%Zd\n", r);
  mpz_clear(r);
  mpz_clear(n);
}
