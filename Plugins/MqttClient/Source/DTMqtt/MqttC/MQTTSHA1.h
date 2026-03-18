// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#if !defined(SHA1_H)
#define SHA1_H

#if defined(OPENSSL)
#include <openssl/sha.h>

/** SHA-1 Digest Length */
#define SHA1_DIGEST_LENGTH SHA_DIGEST_LENGTH

#else /* if defined(OPENSSL) */

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <wincrypt.h>
typedef struct SHA_CTX_S
{
	HCRYPTPROV hProv;
	HCRYPTHASH hHash;
} SHA_CTX;
#else /* if defined(_WIN32) || defined(_WIN64) */

#include <stdint.h>
typedef struct SHA_CTX_S {
	uint32_t h[5];
	union {
		uint32_t w[16];
		uint8_t buffer[64];
	};
	unsigned int size;
	unsigned int total;
} SHA_CTX;
#endif /* else if defined(_WIN32) || defined(_WIN64) */

#include <stddef.h>

/** SHA-1 Digest Length (number of bytes in SHA1) */
#define SHA1_DIGEST_LENGTH (160/8)

/**
 * Initializes the SHA1 hashing algorithm
 *
 * @param[in,out]  ctx                 hashing context structure
 *
 * @see MQTT_SHA1_Update
 * @see MQTT_SHA1_Final
 */
int MQTT_SHA1_Init(SHA_CTX *ctx);

/**
 * Updates a block to the SHA1 hash
 *
 * @param[in,out]  ctx                 hashing context structure
 * @param[in]      data                block of data to hash
 * @param[in]      len                 length of block to hash
 *
 * @see MQTT_SHA1_Init
 * @see MQTT_SHA1_Final
 */
int MQTT_SHA1_Update(SHA_CTX *ctx, const void *data, size_t len);

/**
 * Produce final SHA1 hash
 *
 * @param[out]     md                  SHA1 hash produced (must be atleast
 *                                     @p SHA1_DIGEST_LENGTH in length)
 * @param[in,out]  ctx                 hashing context structure
 *
 * @see MQTT_SHA1_Init
 * @see MQTT_SHA1_Final
 */
int MQTT_SHA1_Final(unsigned char *md, SHA_CTX *ctx);

#endif /* if defined(OPENSSL) */
#endif /* SHA1_H */

