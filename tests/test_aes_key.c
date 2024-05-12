
#include "test_aes_key.h"
#include "aes.h"

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

void test_aes_key(void){
    uint8_t key[256 / 8] = {0};
    AES_KEY_TYPE type = AES_B256;
    Aes_Key* key_p = aes_new_key(type, key);
    assert(key_p);
    aes_delete_key(&key_p);
    assert(key_p == NULL);
}

void test_aes_key_expansion(void) {
    uint8_t key[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
    AES_KEY_TYPE type = AES_B128;
    Aes_Key* key_p = aes_new_key(type, key);
    assert(key_p);

    // check some values
    assert(key_p->key[0] == 0x2b7e1516);
    assert(key_p->key[4] == 0xa0fafe17);
    assert(key_p->key[43] == 0xb6630ca6);

    // cleanup
    aes_delete_key(&key_p);
    assert(key_p == NULL);
}
