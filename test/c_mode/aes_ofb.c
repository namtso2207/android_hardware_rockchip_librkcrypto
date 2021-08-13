#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "aes_core.h"

static void rk_crypto_ofb128_encrypt(const unsigned char *in, unsigned char *out,
			int len, const void *key, unsigned char *ivec, unsigned int *num)
{
	int n, l=0;

	n = *num;

	while (l<len) {
		if (n==0) {
			rk_aes_encrypt(ivec, ivec, key);
		}
		out[l] = in[l] ^ ivec[n];
		++l;
		n = (n+1) % 16;
	}

	*num=n;
}

/* XTS makes use of two different keys, usually generated by splitting
 * the supplied block cipher's key in half.
 * Because of the splitting, users wanting AES 256 and AES 128 encryption
 * will need to choose key sizes of 512 bits and 256 bits respectively.
 */    
int rk_aes_ofb_encrypt(const unsigned char *in, unsigned char *out,
        unsigned long length, const unsigned char *key, const int key_len, 
        unsigned char *ivec, const int enc)
{
		RK_AES_KEY ks;
		unsigned int num = 0;

		if(key_len != 16 && key_len != 24 && key_len != 32)
			return -1;

		if(length%AES_BLOCK_SIZE || length == 0)
			return -1;

        rk_aes_set_encrypt_key(key, key_len * 8, &ks);
		rk_crypto_ofb128_encrypt(in, out, length, &ks, ivec, &num);
		return 0;
}


