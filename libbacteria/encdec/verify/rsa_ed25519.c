#include "encdec/rsa_ed25519.h"
#include "encdec/base64.h"

char *GetStrFromLineToLine(const char fullString[], const char startString[],
                           const char endString[], size_t *retSize,
                           bool fromStartToEnd, bool b64decode) {
  char *pBuf = strstr(fullString, startString);
  char *eBuf = strstr(fullString, endString);
  if (!fromStartToEnd) {
    pBuf = strstr(pBuf, "\n");
    pBuf += 1;
  } else {
    eBuf = strstr(eBuf, "\n");
  }
  size_t len = eBuf - pBuf;
  char tmp[len + 1];
  bzero(tmp, len);
  for (unsigned int i = 0; i < len && *pBuf != '\0' && pBuf != eBuf; i++) {
    if (!fromStartToEnd && (*(pBuf) == '\n' || (*(pBuf) == ' '))) {
      pBuf++;
      i--;
      continue;
    }
    tmp[i] = *(pBuf++);
  }
  tmp[len] = 0;
  //		printf("tmp: %s\n",tmp);
  if (b64decode) {
    char *base64DecodeOutput;
    Base64Decode(tmp, (unsigned char **)&base64DecodeOutput, retSize);
    return base64DecodeOutput;
  } else {
    char *retbuf = malloc(sizeof(char) * len + 1);
    memcpy(retbuf, tmp, len);
    retbuf[len] = 0;
    *retSize = len;
    return retbuf;
  }
}
struct aKeyPair generateKeysEd25519(FILE *exitFile) { // 2056, 3
  struct aKeyPair rt;
  rt.type = ed25519;

  EVP_PKEY *pkey = NULL;
  EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, NULL);
  EVP_PKEY_keygen_init(pctx);
  EVP_PKEY_keygen(pctx, &pkey);

  // RSA *rsa = RSA_generate_key_ex(kBits, kExp, 0, 0);

  /* To get the C-string PEM form: */

  INITBIO();
  BIOTOFILE();
  BIOTORT();
  FREEBIO();

  INITKEYS();

  EVP_PKEY_CTX_free(pctx);
  rt.pkey = pkey;
  // EVP_PKEY_free(pkey);
  return rt;
}

struct aKeyPair generateKeysRSA(unsigned int bits, unsigned int primes,
                                FILE *exitFile) { // 2056, 3
  struct aKeyPair rt;
  rt.type = aRSA;
  if (bits > OPENSSL_RSA_MAX_MODULUS_BITS) {
    fprintf(
        stderr,
        "Warning: It is not recommended to use more than %d bit for RSA keys.\n"
        "         Your key size is %d! Larger key size may behave not as "
        "expected.\n",
        OPENSSL_RSA_MAX_MODULUS_BITS, bits);
  }

  // BN_GENCB *cb = BN_GENCB_new();
  // ENGINE *eng = NULL;
  // BIGNUM *bn = BN_new();
  // BIO *out = NULL;

  // EVP_CIPHER *enc = NULL;
  //    if (bn == NULL || cb == NULL){
  //	fprintf(stderr,"Can't initialize BN/CB openssl!\n");
  //	return rt;
  //    }
  EVP_PKEY *pkey = NULL;
  EVP_PKEY_CTX *ctx = NULL;
  ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
  //    EVP_PKEY_CTX_set_cb(ctx, genrsa_cb);

  if (!ctx) {
    fprintf(stderr, "cant init ctx EVP_PKEY_RSA\n");
    return rt;
  }
  if (EVP_PKEY_keygen_init(ctx) <= 0) {
    fprintf(stderr, "cant init keygen EVP_PKEY_RSA\n");
    return rt;
  }
  if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits) <= 0) {
    EVP_PKEY_CTX_free(ctx);
    fprintf(stderr, "cant init keygen bits\n");
    return rt;
  }
  if (EVP_PKEY_CTX_set_rsa_keygen_primes(ctx, primes) <= 0) {
    EVP_PKEY_CTX_free(ctx);
    fprintf(stderr, "err settings RSA primes\n");
    return rt;
  }

  if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
    EVP_PKEY_CTX_free(ctx);
    fprintf(stderr, "err keygen pkey\n");
  }

  INITBIO();
  BIOTOFILE();
  BIOTORT();
  FREEBIO();
  INITKEYS();
  EVP_PKEY_CTX_free(ctx);
  rt.pkey = pkey;
  return rt;
}

struct aKeyPair initPrivKey(const char *filepath,
                            enum aTypes type /*,const char * password*/) {
  struct aKeyPair rt;
  EVP_PKEY *pkey = NULL;
  EVP_PKEY_CTX *ctx = NULL;
  rt.type = type;
  //  if (type == aRSA)
  //    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
  //  else
  //    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, NULL);
  FILE *f = fopen(filepath, "rb");
  if (f == NULL) {
    fprintf(stderr, "Can't open file %s\n", filepath);
    return rt;
  }
  PEM_read_PrivateKey(f, &pkey, NULL, NULL);
  rt.pkey = pkey;
  fclose(f);

  INITBIO();
  // BIOTOFILE();
  BIOTORT();
  FREEBIO();
  INITKEYS();
  //  EVP_PKEY_CTX_free(ctx);

  return rt;
}
#define ERRSIGN(msg)                                                           \
  EVP_MD_CTX_destroy(md);                                                      \
  fprintf(stderr, msg);                                                        \
  return 0;

size_t singIt(const char *plaintext, size_t plaintext_size, uint8_t *sigret,
              EVP_PKEY *key, const EVP_MD *mdalgo, enum aTypes type) {
  EVP_MD_CTX *md;
  if (!(md = EVP_MD_CTX_create())) {
    fprintf(stderr, "Error MD CTX create\n");
    return 0;
  }
  if (type == aRSA) {
    if (EVP_DigestSignInit(md, NULL, mdalgo, NULL, key) != 1) {
      EVP_MD_CTX_destroy(md);
      fprintf(stderr, "Error in EVP_DigestSignInit.\n");
      return 0;
    }

    if (EVP_DigestSignUpdate(md, (char *)plaintext, plaintext_size) != 1) {
      EVP_MD_CTX_destroy(md);
      fprintf(stderr, "Error computing hash on document.\n");
      return 0;
    }
    size_t s_len;
    if (EVP_DigestSignFinal(md, NULL, &s_len) != 1) { // Segfault here
      EVP_MD_CTX_destroy(md);
      fprintf(stderr, "Error determining maximum signature size.\n");
      return 0;
    }
    if (EVP_DigestSignFinal(md, sigret, &s_len) != 1) { // or here (or both)
      ERRSIGN("Error in signing DigestSignFinal.\n");
    }
    EVP_MD_CTX_destroy(md);
    return s_len;
  } else {
    if (EVP_DigestSignInit(md, NULL, NULL, NULL, key) != 1) {
      ERRSIGN("Error in EVP_DigestSignInit.\n");
    }

    size_t l = 64;
    uint8_t sig[l]; // temporary buffer for signature. openssl issue #7232
    bzero(sig, l);
    // bzero(sigret, plaintext_size);
    if (EVP_DigestSign(md, sig, &l, plaintext, plaintext_size) != 1) {
      ERRSIGN("Error in EVP_DigestSign.\n");
    }
    memcpy(sigret, sig, l);
    sigret[l] = 0;
    EVP_MD_CTX_destroy(md);
    return l;
  }
}
#undef ERRSIGN

int verifyIt(uint8_t *signature, size_t signature_size,
             const uint8_t *plaintext, size_t plaintext_size,
             uint8_t *signingkey, size_t signingkey_size, const EVP_MD *mdalgo,
             enum aTypes type) {

  EVP_MD_CTX *md;
  if (!(md = EVP_MD_CTX_create())) {
    fprintf(stderr, "Error MD CTX create\n");
    return 0;
  }
  EVP_PKEY *pkey;
  RSA *rsa = NULL;
  BIO *keybio;

  if (type != aRSA) {
    pkey = EVP_PKEY_new_raw_public_key(EVP_PKEY_ED25519, NULL, signingkey,
                                       signingkey_size);
  } else {
    pkey = EVP_PKEY_new();
    keybio = BIO_new_mem_buf((void *)signingkey, -1);
    if (keybio == NULL) {
      return 0;
    }
    PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
    //	  printf("%p RSA\n", rsa);

    EVP_PKEY_assign_RSA(pkey, rsa);
  }

  // printf("%p key\n", pkey);
  int rtv = -1;

  if (type != aRSA) {

    if (EVP_DigestVerifyInit(md, NULL, NULL, NULL, pkey) == 1) {
      // puts("set rtv");
      rtv = EVP_DigestVerify(md, signature, 64, plaintext, plaintext_size);
    }
    if (rtv == -1)
      fprintf(stderr, "Error in EVP_DigestVerifyInit.\n");
  } else {
    if (1 == EVP_DigestVerifyInit(md, NULL, mdalgo, NULL, pkey))
      if (1 == EVP_DigestVerifyUpdate(md, plaintext, plaintext_size)) {
        rtv = EVP_DigestVerifyFinal(md, signature, signature_size);
      }
  }
  if (rtv == -1)
    fprintf(stderr, "verify error\n");
  EVP_MD_CTX_destroy(md);
  EVP_PKEY_free(pkey);
  BIO_free_all(keybio);

  //	if(rsa != NULL)
  // RSA_free(rsa);
  return rtv;
}
