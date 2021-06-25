#include<gmp.h>
#include<stdio.h>
int main(int cn, char**v){
  if(cn != 3) return fprintf(stderr,"%s a b\n",v[0]);
  mpf_t x, y, c;
  mpf_init (c);
  mpf_init (x);           /* use default precision */
  mpf_init2 (y, 256);     /* precision at least 256 bits */
  mpf_set_str(x, v[1], 10);
  mpf_set_str(y, v[2], 10);
  mpf_div(c, x,y);
  mpf_out_str(stdout, 10, 65535, c);
  puts("");
  /* Unless the program is about to exit, do ... */
  mpf_clear (x);
  mpf_clear (y);
}
