
// #------------------------------#
// |          includes            |
// #------------------------------#
// c includes

// my includes
#include "aes.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// #------------------------------#
// |           macros             |
// #------------------------------#

// #------------------------------#
// | global variables definitions |
// #------------------------------#

// #------------------------------#
// | static variables definitions |
// #------------------------------#

// #------------------------------#
// | static functions declarations|
// #------------------------------#


// #------------------------------#
// | global function definitions  |
// #------------------------------#
Aes_Key* aes_new_key(AES_KEY_TYPE type, const uint8_t* keys){
    assert(type > 0); // check for negative data input
    const size_t key_size = type;

    Aes_Key* key_p = malloc(sizeof(Aes_Key) + key_size); // FAM
    key_p->type = type;
    memcpy(key_p->key, keys, key_size);

    return key_p;
}
void aes_delete_key(Aes_Key** aes_key_pp) {
    if(aes_key_pp == NULL || *aes_key_pp == NULL){
        return;
    }
    Aes_Key* aes_key_p = *aes_key_pp;
    free(aes_key_p);
    aes_key_p = NULL;
}

// #------------------------------#
// | static functions definitions |
// #------------------------------#
