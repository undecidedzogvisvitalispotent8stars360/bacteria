#include <openssl/bio.h>
#include <openssl/bn.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/opensslconf.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <stdio.h>
#include <string.h>

#define INITBIO()                                                              \
  BIO *bio_pub = BIO_new(BIO_s_mem());                                         \
  BIO *bio_priv = BIO_new(BIO_s_mem());                                        \
  PEM_write_bio_PrivateKey(bio_priv, pkey, NULL, NULL, 0, NULL, NULL);         \
  PEM_write_bio_PUBKEY(bio_priv, pkey);

// ToDO: password!!!!
#define BIOTOFILE()                                                            \
  if (exitFile != NULL) {                                                      \
    PEM_write_PrivateKey(exitFile, pkey, NULL, NULL, 0, NULL, NULL);           \
  }

#define BIOTORT()                                                              \
  rt.privKeyLen = BIO_pending(bio_priv);                                       \
  rt.pubKeyLen = BIO_pending(bio_pub);                                         \
  rt.pubKey = malloc(rt.pubKeyLen + 1 * sizeof(char));                         \
  rt.privKey = malloc(rt.privKeyLen + 1 * sizeof(char));                       \
  BIO_read(bio_pub, rt.pubKey, rt.pubKeyLen);                                  \
  BIO_read(bio_priv, rt.privKey, rt.privKeyLen);                               \
  rt.pubKey[rt.pubKeyLen] = 0;                                                 \
  rt.privKey[rt.privKeyLen] = 0;

#define FREEBIO()                                                              \
  BIO_free_all(bio_priv);                                                      \
  BIO_free_all(bio_pub);

typedef enum { false, true } bool;

enum aTypes { ed25519, aRSA };

struct aKeyPair {
  char *pubKey, *privKey;
  size_t pubKeyLen, privKeyLen;
  EVP_PKEY *pkey;
  enum aTypes type;
};
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

  EVP_PKEY_CTX_free(ctx);
  return rt;
}

struct aKeyPair initPrivKey(const char *filepath,
                            enum aTypes type /*,const char * password*/) {
  struct aKeyPair rt;
  EVP_PKEY *pkey = NULL;
  EVP_PKEY_CTX *ctx = NULL;
  if (type == aRSA)
    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
  else
    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, NULL);
  FILE *f = fopen(filepath, "rb");
  PEM_read_PrivateKey(f, &pkey, NULL, NULL);
  rt.pkey = pkey;
  INITBIO();
  // BIOTOFILE();
  BIOTORT();
  FREEBIO();
  EVP_PKEY_CTX_free(ctx);
  return rt;
}

size_t singIt(const char *plaintext, size_t plaintext_size, uint8_t *sigret,
              EVP_PKEY *key,const EVP_MD * mdalgo,enum aTypes type) {
  EVP_MD_CTX *md;
  if (!(md = EVP_MD_CTX_create())) {
    fprintf(stderr, "Error MD CTX create\n");
    return 0;
  }
  if (type == aRSA) {
    if (EVP_DigestSignInit(md, NULL, mdalgo, NULL, key) != 1) {
      EVP_MD_CTX_destroy(md);
      return fprintf(stderr, "Error in EVP_DigestSignInit.\n");
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
      fprintf(stderr, "Error signing.\n");
    }
    EVP_MD_CTX_destroy(md);
    return s_len;
  } else {
    if (EVP_DigestSignInit(md, NULL, NULL, NULL, key) != 1) {
      EVP_MD_CTX_destroy(md);
      return fprintf(stderr, "Error in EVP_DigestSignInit.\n");
    }

    size_t l = 64;
    uint8_t sig[l]; // temporary buffer for signature. openssl issue #7232
    bzero(sig, l);
    //bzero(sigret, plaintext_size);
    EVP_DigestSign(md, sig, &l, plaintext, plaintext_size);
    memcpy(sigret, sig, l);
    sigret[l]=0; 
    EVP_MD_CTX_destroy(md);
    return l;
  }
}

int verifyIt(uint8_t *signature, size_t signature_size,const uint8_t * plaintext, size_t plaintext_size ,
              uint8_t* signingkey, size_t signingkey_size,const EVP_MD * mdalgo,enum aTypes type){

	  EVP_MD_CTX *md;
	  if (!(md = EVP_MD_CTX_create())) {
	    fprintf(stderr, "Error MD CTX create\n");
	    return 0;
	  }

	EVP_PKEY * pkey = EVP_PKEY_new_raw_public_key( (type==aRSA? EVP_PKEY_RSA : EVP_PKEY_ED25519),NULL, signingkey, signingkey_size);

	printf("%p key\n", pkey);
	int rtv = -1;
	
	if(type != aRSA){
		
		if(EVP_DigestVerifyInit (md, NULL, NULL, NULL, pkey) == 1){
			puts("set rtv");
			rtv = EVP_DigestVerify (md, signature, 64, plaintext, plaintext_size);
		}
		if(rtv == -1) fprintf(stderr, "Error in EVP_DigestVerifyInit.\n");
	}else{
		if(1 == EVP_DigestVerifyInit(md, NULL, mdalgo, NULL, pkey))
			if( 1 == EVP_DigestVerifyUpdate(md, plaintext, plaintext_size) )
				rtv = EVP_DigestVerifyFinal(md, signingkey,signingkey_size);
	}
	if(rtv == -1) fprintf(stderr, "verify error\n");
	EVP_MD_CTX_destroy(md);
	EVP_PKEY_free(pkey);
	return rtv;
	
	

}

int main(void) {
  /*FILE * myPrivKey1 = fopen("privkey.ed25519", "wb");
  struct aKeyPair keyPair = generateKeysEd25519(myPrivKey1);
  FILE * myPrivKey = fopen("privkey.rsa", "wb");
  struct aKeyPair rsaKeyPair  = generateKeysRSA(2056, 3,myPrivKey);
  fclose(myPrivKey);
  fclose(myPrivKey1);*/
  struct aKeyPair keyPair = initPrivKey("privkey.ed25519", ed25519);
  struct aKeyPair rsaKeyPair = initPrivKey("privkey.rsa", aRSA);
  printf("keyPair\n %s\n %s\n", keyPair.pubKey, keyPair.privKey);
  printf("rsaKeyPair\n %s\n %s\n", rsaKeyPair.pubKey, rsaKeyPair.privKey);

  // size_t singIt(const char * plaintext,size_t plaintext_size, char* sigret,
  // EVP_PKEY * key);

  const char msg[] =
      "test msg";
  char retbuf[1024];
  size_t sSize = singIt(msg, sizeof(msg), retbuf, keyPair.pkey, EVP_sha512(),ed25519);
  printf("Sign data: %s\n", retbuf);
  char pubKey[1024];
  size_t keySize;
  struct aKeyPair another_keyPair = generateKeysEd25519(NULL);

  EVP_PKEY_get_raw_public_key(keyPair.pkey, pubKey, &keySize);

  int rtv =verifyIt(retbuf, sSize, msg, sizeof(msg), pubKey, keySize, EVP_sha512(), ed25519);
  if(rtv > 0) puts("Verified!");
  else puts("not verified:(");
  puts("Set another key now...");
  EVP_PKEY_get_raw_public_key(another_keyPair.pkey, pubKey, &keySize);
  rtv =verifyIt(retbuf, sSize, msg, sizeof(msg), pubKey, keySize, EVP_sha512(), ed25519);
  if(rtv > 0) puts("Verified!");
  else puts("not verified:(");
  EVP_PKEY_free(keyPair.pkey);
  free(keyPair.privKey);
  free(keyPair.pubKey);

  EVP_PKEY_free(rsaKeyPair.pkey);
  free(rsaKeyPair.privKey);
  free(rsaKeyPair.pubKey);

  EVP_PKEY_free(another_keyPair.pkey);
  free(another_keyPair.pubKey);
  free(another_keyPair.privKey);
}
