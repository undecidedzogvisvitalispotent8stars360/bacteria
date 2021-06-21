#include"AES.h"

int 
AES_any_decrypt(aes_enctype enc,unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        HANDLERRSSL();;

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, enc(), NULL, key, iv))
        HANDLERRSSL();;

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        HANDLERRSSL();;
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        HANDLERRSSL();;
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

int 
AES_any_encrypt(aes_enctype enc,unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        HANDLERRSSL();;

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_EncryptInit_ex(ctx, enc(), NULL, key, iv))
        HANDLERRSSL();;

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        HANDLERRSSL();;
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        HANDLERRSSL();;
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}


CREATEAESALGO(AES_256_cbc, EVP_aes_256_cbc);
CREATEAESALGO(AES_256_ecb, EVP_aes_256_ecb);




int 
main(int count, char ** values){
    /*
     * Set up the key and iv. Do I need to say to not hard code these in a
     * real application? :-)
     */
    if(count != 5){
	return fprintf(stderr,"%s msg key iv ENCType(2,4,2|4[6])\n",values[0]);
    }
    unsigned char key[32];
    unsigned char iv[16];
    memcpy(key, values[2], sizeof(key));
    memcpy(iv, values[3], sizeof(iv));

    unsigned int enctype = atoi(values[4]);

    /* A 256 bit key */
   // unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

    /* A 128 bit IV */
    //unsigned char *iv = (unsigned char *)"0123456789012345";

    /* Message to be encrypted */
    unsigned char *plaintext =
        (unsigned char *)values[1];

    /*
     * Buffer for ciphertext. Ensure the buffer is long enough for the
     * ciphertext which may be longer than the plaintext, depending on the
     * algorithm and mode.
     */
    unsigned char ciphertext[128];

    /* Buffer for the decrypted text */
    unsigned char decryptedtext[128];

    int decryptedtext_len, ciphertext_len;

    if( (enctype&t_cbc) == t_cbc ){
	puts("t_cbc");
	ciphertext_len = AES_256_cbc_encrypt (plaintext, strlen ((char *)plaintext), key, iv,
                              ciphertext);
	/* Do something useful with the ciphertext here */
	printf("Ciphertext is:\n");
        BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);
    	/* Decrypt the ciphertext */
    	decryptedtext_len = AES_256_cbc_decrypt(ciphertext, ciphertext_len, key, iv,
                                decryptedtext);
    	/* Add a NULL terminator. We are expecting printable text */
    	decryptedtext[decryptedtext_len] = '\0';
  	  /* Show the decrypted text */
  	printf("Decrypted text is:\n");
  	printf("%s\n", decryptedtext);
    }
    if( (enctype&t_ecb) == t_ecb ){
	puts("t_ecb");
	ciphertext_len = AES_256_ecb_encrypt (plaintext, strlen ((char *)plaintext), key, iv,
                              ciphertext);
	/* Do something useful with the ciphertext here */
	printf("Ciphertext is:\n");
        BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);
    	/* Decrypt the ciphertext */
    	decryptedtext_len = AES_256_ecb_decrypt(ciphertext, ciphertext_len, key, iv,
                                decryptedtext);
    	/* Add a NULL terminator. We are expecting printable text */
    	decryptedtext[decryptedtext_len] = '\0';
  	  /* Show the decrypted text */
  	printf("Decrypted text is:\n");
  	printf("%s\n", decryptedtext);
    }

    /* Encrypt the plaintext */
    






    return 0;

}