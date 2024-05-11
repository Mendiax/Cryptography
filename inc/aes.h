#ifndef __CRYPTOGRAPHY_AES_H__
#define __CRYPTOGRAPHY_AES_H__
// #-------------------------------#
// |           includes            |
// #-------------------------------#
// c includes
#include <stdint.h>

// my includes
#include "aes_key.h"

// #-------------------------------#
// |            macros             |
// #-------------------------------#

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

// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#

#endif
