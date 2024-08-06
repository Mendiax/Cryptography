#ifndef __CRYPTOGRAPHY_AES_H__
#define __CRYPTOGRAPHY_AES_H__
// #-------------------------------#
// |           includes            |
// #-------------------------------#
// c includes
#include <stdint.h>

// my includes
#include "aes/aes_key.h"

// #-------------------------------#
// |            macros             |
// #-------------------------------#

#define AES_ALIGN_LENGTH(l) (((l + 15)/16) * 16)
#define AES_IS_ALIGN_LENGTH(l) ((l) % 16 == 0)


// #-------------------------------#
// | global types declarations     |
// #-------------------------------#

// #-------------------------------#
// | global variables declarations |
// #-------------------------------#

// #-------------------------------#
// | global function declarations  |
// #-------------------------------#


/**
 * @brief Encrypt single 128-bit block
 *
 * @param input
 * @param key_p
 * @param output
 */
void aes_encrypt_block(const uint8_t *input, const Aes_Key *key_p, uint8_t *output);

/**
 * @brief Decrypt single 128-bit block
 *
 * @param input
 * @param key_p
 * @param output
 */
void aes_decrypt_block(const uint8_t *input, const Aes_Key *key_p, uint8_t *output);

void aes_encrypt_aligned(const uint8_t *input, size_t input_len, const Aes_Key *key_p, uint8_t *output);
void aes_decrypt_aligned(const uint8_t *input, size_t input_len, const Aes_Key *key_p, uint8_t *output);

// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#

#endif
