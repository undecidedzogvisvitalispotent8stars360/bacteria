#include <stdio.h>
#include <openssl/bn.h>

int main (int c, char **v)
{
  if(c != 3) return fprintf(stderr, "%s first second\n", v[0]);
  //static const
  //char p_str[] = "82019154470699086128524248488673846867876336512717";

  BIGNUM *first = BN_new();
  BIGNUM *second = BN_new();
  BIGNUM * rem = BN_new();
  BN_CTX* ctx = BN_CTX_new();
  BN_dec2bn(&first, v[1]);
  BN_dec2bn(&second, v[2]);
  BIGNUM * result = BN_new();
 // BN_mul(result, first,second, ctx);
  BN_div(result, rem, first, second,
         ctx);

  char * number_str = BN_bn2dec(rem);
  printf("%s\n", number_str);

  OPENSSL_free(number_str);
  BN_free(first);
  BN_free(second);
  BN_free(result);
  return 0;
}

