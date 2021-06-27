#include"rsa_ed25519.h"
int main(void) {
  FILE * tmp = fopen("privkey.ed25519", "rb");
  if(tmp == NULL){
	  FILE * myPrivKey1 = fopen("privkey.ed25519", "wb");
	  struct aKeyPair keyPair = generateKeysEd25519(myPrivKey1);
	  FILE * myPrivKey = fopen("privkey.rsa", "wb");
	  struct aKeyPair rsaKeyPair  = generateKeysRSA(2056, 3,myPrivKey);
	  fclose(myPrivKey);
	  fclose(myPrivKey1);
  }else fclose(tmp);
 
  struct aKeyPair keyPair = initPrivKey("privkey.ed25519", ed25519);
  struct aKeyPair rsaKeyPair = initPrivKey("privkey.rsa", aRSA);
  printf("keyPair\n %s\n %s\n", keyPair.pubKey, keyPair.privKey);
  printf("rsaKeyPair\n %s\n %s\n", rsaKeyPair.pubKey, rsaKeyPair.privKey);

  // size_t singIt(const char * plaintext,size_t plaintext_size, char* sigret,
  // EVP_PKEY * key);

  const char msg[] =
      "test msg";
  char retbuf[1024];
  puts("Test RSA");
  size_t sSize = singIt(msg, sizeof(msg), retbuf, rsaKeyPair.pkey, EVP_sha512(),aRSA);
  printf("Sign data: %s\n", retbuf);
  char pubKey[1024];
  size_t keySize;
  struct aKeyPair another_keyPair = generateKeysRSA(2048, 3, NULL);
  printf("aKeyPair\npub->%s\n priv->%s\n", another_keyPair.pubKey, another_keyPair.privKey);
 

  int rtv =verifyIt(retbuf, sSize, msg, sizeof(msg), rsaKeyPair.pubKey, keyPair.pubKeyLen, EVP_sha512(), aRSA);
  if(rtv > 0) puts("Verified!");
  else puts("not verified:(");
  puts("Set another key now...");

  rtv =verifyIt(retbuf, sSize, msg, sizeof(msg), another_keyPair.pubKey, another_keyPair.pubKeyLen, EVP_sha512(), aRSA);
  if(rtv > 0) puts("Verified!");
  else puts("not verified:(");
  puts("Test ed25519");
  sSize = singIt(msg, sizeof(msg), retbuf, keyPair.pkey, EVP_sha512(),ed25519);
  struct aKeyPair another_keyPaired25519 = generateKeysEd25519(NULL);

  rtv =verifyIt(retbuf, sSize, msg, sizeof(msg), keyPair.pubKey, keyPair.pubKeyLen, EVP_sha512(), ed25519);
  if(rtv > 0) puts("Verified!");
  else puts("not verified:(");
  puts("Set another key now...");

  rtv =verifyIt(retbuf, sSize, msg, sizeof(msg), another_keyPaired25519.pubKey, another_keyPaired25519.pubKeyLen, EVP_sha512(), ed25519);
  if(rtv > 0) puts("Verified!");
  else puts("not verified:(");
  puts("Will be Verified-Not verified; Verified-Not verified;");

  EVP_PKEY_free(keyPair.pkey);
  free(keyPair.privKey);
  free(keyPair.pubKey);

  EVP_PKEY_free(rsaKeyPair.pkey);
  free(rsaKeyPair.privKey);
  free(rsaKeyPair.pubKey);

  EVP_PKEY_free(another_keyPair.pkey);
  free(another_keyPair.pubKey);
  free(another_keyPair.privKey);
 
  EVP_PKEY_free(another_keyPaired25519.pkey);
  free(another_keyPaired25519.pubKey);
  free(another_keyPaired25519.privKey);
}