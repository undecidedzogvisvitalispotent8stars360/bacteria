#pragma once
#include <openssl/bio.h>
#include <openssl/bn.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/opensslconf.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>



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

#define INITKEYS()\
  if(rt.pubKeyLen == 0 && rt.privKeyLen > 0){\
	size_t privKeySize;		\
	if(rt.type != aRSA){\
  		size_t keySize;\
		EVP_PKEY_get_raw_public_key(pkey, NULL, &keySize);\
		if(keySize  <= 0) {\
			fprintf(stderr, "Can't init key!\n");\
			return rt;\
		}\
		char tmpKey[keySize];\
		EVP_PKEY_get_raw_public_key(pkey, tmpKey, &keySize);\
		free(rt.pubKey);\
		rt.pubKeyLen = keySize;\
		rt.pubKey = malloc(keySize*sizeof(char)+1);\
		memcpy(rt.pubKey, tmpKey, keySize);\
		rt.pubKey[keySize]=0;\
		EVP_PKEY_get_raw_private_key(pkey,NULL,&keySize);\
		char tmpKeyPriv[keySize];\
		EVP_PKEY_get_raw_private_key(pkey,tmpKeyPriv,&keySize);\
		free(rt.privKey);\
		rt.privKey = malloc(sizeof(char)*keySize+1);\
		rt.privKeyLen=keySize;\
		memcpy(rt.privKey, tmpKeyPriv, keySize);\
		rt.privKey[keySize]=0;\
	}else{\
		size_t privKeyLen, pubKeyLen;\
		unsigned char * privKey = GetStrFromLineToLine(rt.privKey, "-----BEGIN PRIVATE KEY-----", "-----END PRIVATE KEY-----", &privKeyLen,true,false);\
		unsigned char * pubKey = GetStrFromLineToLine(rt.privKey, "-----BEGIN PUBLIC KEY-----", "-----END PUBLIC KEY-----", &pubKeyLen,true,false);\
		free(rt.privKey);\
		rt.privKey = malloc(sizeof(char)*privKeyLen+1);\
		rt.privKeyLen=privKeyLen;\
		rt.privKey[privKeyLen] = 0;\
		memcpy(rt.privKey, privKey, privKeyLen);\
		free(rt.pubKey);\
		rt.pubKey = malloc(sizeof(char)*pubKeyLen+1);\
		rt.pubKeyLen=pubKeyLen;\
		rt.pubKey[pubKeyLen] = 0;\
		memcpy(rt.pubKey, pubKey, pubKeyLen);\
	}\
  }

#ifndef bool
	typedef enum { false, true } bool;
#endif

enum aTypes { ed25519, aRSA };

struct aKeyPair {
  char *pubKey, *privKey;
  size_t pubKeyLen, privKeyLen;
  EVP_PKEY *pkey;
  enum aTypes type;
};

char * GetStrFromLineToLine(const char fullString[], const char startString[], const char endString[], size_t * retSize, bool fromStartToEnd, bool b64decode);
struct aKeyPair generateKeysEd25519(FILE *exitFile); // 2056, 3
struct aKeyPair generateKeysRSA(unsigned int bits, unsigned int primes,
                                FILE *exitFile);
struct aKeyPair initPrivKey(const char *filepath,
                            enum aTypes type /*,const char * password*/) ;

size_t singIt(const char *plaintext, size_t plaintext_size, uint8_t *sigret,
              EVP_PKEY *key,const EVP_MD * mdalgo,enum aTypes type);
int verifyIt(uint8_t *signature, size_t signature_size,const uint8_t * plaintext, size_t plaintext_size ,
              uint8_t* signingkey, size_t signingkey_size,const EVP_MD * mdalgo,enum aTypes type);



