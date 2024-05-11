#ifndef __CRYPTOGRAPHY_AES_KEY_H__
#define __CRYPTOGRAPHY_AES_KEY_H__
// #-------------------------------#
// |           includes            |
// #-------------------------------#
// c includes
#include <stdint.h>
#include <stddef.h>

// my includes

// #-------------------------------#
// |            macros             |
// #-------------------------------#

// #-------------------------------#
// | global types declarations     |
// #-------------------------------#
typedef enum AES_KEY_TYPE{
    AES_NONE,
    AES_B128=128,
    AES_B192=192,
    AES_B256=256
} AES_KEY_TYPE;

typedef struct Aes_Key{
    AES_KEY_TYPE type;
    uint8_t key[];
} Aes_Key;

// #-------------------------------#
// | global variables declarations |
// #-------------------------------#

// #-------------------------------#
// | global function declarations  |
// #-------------------------------#

/**
 * @brief Creates new AES key
 *
 * @param type
 * @param keys
 * @return Aes_Key*
 */
Aes_Key* aes_new_key(AES_KEY_TYPE type, const uint8_t* keys);

/**
 * @brief Deletes key
 *
 * @param aes_key_pp
 */
void aes_delete_key(Aes_Key** aes_key_pp);

size_t get_number_of_rounds(const Aes_Key* key_p);

void get_key_expansion(const Aes_Key* key_p, uint32_t* w_out);


// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#

#endif
