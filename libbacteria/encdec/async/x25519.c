#include "encdec/x25519.h"
static size_t len_key = LENKEY;
struct keysPair createKeyPair(const uint8_t *priv, const uint8_t *pub) {
  struct keysPair ret;
  bzero(ret.pubKey, sizeof(ret.pubKey));
  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(NID_X25519, NULL);
  if (!ctx) {
    fprintf(stderr, "Cant create x25519 pair\n");
    return ret;
  }
  EVP_PKEY *privKey =
      EVP_PKEY_new_raw_private_key(EVP_PKEY_X25519, NULL, priv, len_key);
  getRawPubKey(privKey, ret.pubKey);
  // EVP_PKEY_get_raw_public_key(privKey, ret.pubKey, &len_key);
  ret.privKey = privKey;
  // ret.pubKey = pubKey;
  EVP_PKEY_CTX_free(ctx);
  ctx = EVP_PKEY_CTX_new(privKey, NULL);
  ret.pKeyCtx = ctx;
  return ret;
}

void getRawPrivKey(EVP_PKEY *privKey, uint8_t *priv) {
  EVP_PKEY_get_raw_private_key(privKey, priv, &len_key);
}
void getRawPubKey(EVP_PKEY *privKey, uint8_t *pub) {
  EVP_PKEY_get_raw_public_key(privKey, pub, &len_key);
}

void freeKeyPair(struct keysPair *pair) {
  EVP_PKEY_CTX_free(pair->pKeyCtx);
  EVP_PKEY_free(pair->privKey);
  //	EVP_PKEY_free(pair->pubKey);
}
void freeSharedKey(uint8_t *w) {
  if (w != NULL)
    OPENSSL_free(w);
}

void freeSharedKeys(uint8_t *w, ...) {
  va_list ap;
  va_start(ap, w);
  while (*w) {
    freeSharedKey(w);
  }
  va_end(ap);
}

struct keysPair generateKeyPair(void) {
  struct keysPair ret;
  EVP_PKEY_CTX *ctx;
  EVP_PKEY *pkey = NULL;
  ctx = EVP_PKEY_CTX_new_id(NID_X25519, NULL);
  if (!ctx) {
    fprintf(stderr, "Cant create x25519 pair\n");
    return ret;
  }
  if (EVP_PKEY_keygen_init(ctx) <= 0) {
    fprintf(stderr, "can't keygen x25519 pair\n");
    EVP_PKEY_CTX_free(ctx);
    return ret;
  }
  EVP_PKEY_keygen(ctx, &pkey);
  EVP_PKEY_CTX_free(ctx);
  ctx = EVP_PKEY_CTX_new(pkey, NULL);
  bzero(ret.pubKey, sizeof(ret.pubKey));
  EVP_PKEY_get_raw_public_key(pkey, ret.pubKey, &len_key);
  // EVP_PKEY_CTX_free (ctx);

  ret.privKey = pkey;
  // ret.pubKey=pubKey;
  ret.pKeyCtx = ctx;
  return ret;
}

uint8_t *getSharedKey(struct keysPair *pair, const uint8_t *pubPeer,
                      size_t *skeylen) {
  if (!pubPeer || (pubPeer[31] & 0x80)) {
    fprintf(stderr, "Is not pubkey!\n");
    return NULL;
  } // not x25519 key

  if (EVP_PKEY_derive_init(pair->pKeyCtx) <= 0) {
    fprintf(stderr, "derive init error\n");
    return NULL;
  }

  EVP_PKEY *pkey =
      EVP_PKEY_new_raw_public_key(NID_X25519, NULL, pubPeer, len_key);

  if (EVP_PKEY_derive_set_peer(pair->pKeyCtx, pkey) <= 0) {
    fprintf(stderr, "Set peer key error\n");
    EVP_PKEY_free(pkey);
    return NULL;
  }
  if (EVP_PKEY_derive(pair->pKeyCtx, NULL, skeylen) <= 0) {
    fprintf(stderr, "buffer length derive err\n");
    EVP_PKEY_free(pkey);
    return NULL;
  }
  uint8_t *ret = OPENSSL_malloc(*skeylen);
  if (ret == 0) {
    fprintf(stderr, "OPENSSL malloc err\n");
    EVP_PKEY_free(pkey);
    return NULL;
  }
  if (EVP_PKEY_derive(pair->pKeyCtx, ret, skeylen) <= 0) {
    fprintf(stderr, "shared key write err\n");
    EVP_PKEY_free(pkey);
    return NULL;
  }
  EVP_PKEY_free(pkey);
  return ret;
}

static int x25519_bacteria_test(void) {
  struct keysPair pair = generateKeyPair();
  struct keysPair pair1 = generateKeyPair();
  if (pair.pKeyCtx == NULL || pair1.pKeyCtx == NULL) {
    return fprintf(stderr, "can't CTX init\n");
  }
  printf("pubKey: %s\nLen(strlen): %d\n", pair.pubKey,
         (int)strlen(pair.pubKey));
  printf("pubKey1: %s\nLen(strlen): %d\n", pair1.pubKey,
         (int)strlen(pair1.pubKey));
  puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  size_t skeylen, skeylen1;

  // bzero(shared0,sizeof(shared0));
  // bzero(shared1,sizeof(shared0));
  uint8_t *shared0 = getSharedKey(&pair, pair1.pubKey, &skeylen);
  uint8_t *shared1 = getSharedKey(&pair1, pair.pubKey, &skeylen1);
  printf("shared0: %s \nshared1: %s \n", shared0, shared1);
  freeSharedKeys(shared0, shared1, NULL);
  freeKeyPair(&pair);
  freeKeyPair(&pair1);
}
