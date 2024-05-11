#ifndef __CRYPTOGRAPHY_AES_H__
#define __CRYPTOGRAPHY_AES_H__
// #-------------------------------#
// |           includes            |
// #-------------------------------#
// c includes
#include <stdint.h>

// my includes

// #-------------------------------#
// |            macros             |
// #-------------------------------#

// #-------------------------------#
// | global types declarations     |
// #-------------------------------#
typedef enum AES_KEY_TYPE{
    NONE,
    B128=128,
    B192=192,
    B256=256
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

Aes_Key* aes_new_key(AES_KEY_TYPE type, const uint8_t* keys);
void aes_delete_key(Aes_Key** aes_key_pp);

// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#

#endif
