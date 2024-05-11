
// #------------------------------#
// |          includes            |
// #------------------------------#
// c includes
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// my includes
#include "aes_key.h"

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
static inline uint32_t get_round_constant(size_t i);
static inline uint32_t sub_word(uint32_t word);
static inline uint32_t rot_word(uint32_t word);
static void initialize_aes_sbox(uint8_t sbox[256]);

// #------------------------------#
// | global function definitions  |
// #------------------------------#
Aes_Key* aes_new_key(AES_KEY_TYPE type, const uint8_t* keys){
    assert(type > 0); // check for negative data input
    const size_t key_size_bytes = type / 8;

    Aes_Key* key_p = malloc(sizeof(Aes_Key) + key_size_bytes); // FAM
    key_p->type = type;
    memcpy(key_p->key, keys, key_size_bytes);

    return key_p;
}
void aes_delete_key(Aes_Key** aes_key_pp) {
    if(aes_key_pp == NULL || *aes_key_pp == NULL){
        return;
    }
    free(*aes_key_pp);
    *aes_key_pp = NULL;
}

size_t get_number_of_rounds(const Aes_Key* key_p){
    switch (key_p->type) {
        case AES_B128: return 10;
        case AES_B192: return 12;
        case AES_B256: return 14;
        default:
        break;
    }
    // should not be there
    assert(0);
    return 0;
}

/**
 * @brief Get the key expansion array
 *
 * @param key_p
 * @param w_out
 */
void get_key_expansion(const Aes_Key* key_p, uint32_t* w_out){
    const size_t nk = key_p->type / 32;
    uint32_t temp;
    size_t i = 0;
    while (i < nk){
        w_out[i] = (uint32_t)key_p->key[4*i] << 24 | (uint32_t)key_p->key[4*i + 1] << 16 |(uint32_t)key_p->key[4*i + 2] << 8 | (uint32_t)key_p->key[4*i + 3];
        i = i+1;
    }
    i = nk;

    const size_t nr = get_number_of_rounds(key_p);
    while (i < 4 * (nr+1)) {
        temp = w_out[i-1];
        if (i % nk == 0) {
            temp = sub_word(rot_word(temp)) ^ get_round_constant(i/nk);
        }
        else if (nk > 6 && i % nk == 4) {
            temp = sub_word(temp);
        }
        w_out[i] = w_out[i-nk] ^ temp;
        i = i + 1;
    }
}

// #------------------------------#
// | static functions definitions |
// #------------------------------#

static inline uint32_t get_round_constant(size_t i) {
    assert(i <= 10 && i > 0);
    uint8_t rci_constatnts[] = {1, 2, 4, 8, 16, 32, 64, 128, 27, 54};
    const uint32_t rci = rci_constatnts[i-1];

    return  rci << (3*8);
}

static inline uint32_t sub_word(uint32_t word){
    static uint8_t sbox[256] = {0};
    if(sbox[0] == 0) {
        // sbox not initialized
        initialize_aes_sbox(sbox);
    }
    uint32_t word_out;
    uint8_t* words = (uint8_t*)&word;
    uint8_t* words_out = (uint8_t*)&word_out;

    for(size_t i = 0; i < 4; i++) {
        words_out[i] = sbox[words[i]];
    }
    return word_out;
}

static inline uint32_t rot_word(uint32_t word){
    return (word << 8) | (word >> 24);
}

static void initialize_aes_sbox(uint8_t sbox[256]) {
	uint8_t p = 1, q = 1;
    #define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))

	do {
		p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);

		q ^= q << 1;
		q ^= q << 2;
		q ^= q << 4;
		q ^= q & 0x80 ? 0x09 : 0;

		uint8_t xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);

		sbox[p] = xformed ^ 0x63;
	} while (p != 1);

	/* 0 is a special case since it has no inverse */
	sbox[0] = 0x63;
    #undef ROTL8
}
