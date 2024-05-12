
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
/**
 * @brief Expand key
 *
 * @param key_p
 * @param type
 * @param w_out
 */
static void get_key_expansion(const uint8_t* key_p,
                       const AES_KEY_TYPE type,
                       uint32_t* w_out);

static inline uint32_t get_round_constant(size_t i);
static inline uint32_t sub_word(uint32_t word);
static inline uint32_t rot_word(uint32_t word);
static void initialize_aes_sbox(uint8_t sbox[256]);

// #------------------------------#
// | global function definitions  |
// #------------------------------#
Aes_Key* aes_new_key(AES_KEY_TYPE type, const uint8_t* keys){
    assert(type > 0); // check for negative data input
    // const size_t key_size_bytes = type / 8;

    const size_t nr = aes_type_get_number_of_rounds(type);
    const size_t expansion_key_size = NB*(nr+1) * sizeof(uint32_t);

    Aes_Key* key_p = malloc(sizeof(Aes_Key) + expansion_key_size); // FAM
    assert(key_p);
    memset(key_p, 0, sizeof(Aes_Key) + expansion_key_size);

    key_p->type = type;
    get_key_expansion(keys, key_p->type, key_p->key);

    return key_p;
}
void aes_delete_key(Aes_Key** aes_key_pp) {
    if(aes_key_pp == NULL || *aes_key_pp == NULL){
        return;
    }
    free(*aes_key_pp);
    *aes_key_pp = NULL;
}

size_t aes_type_get_number_of_rounds(AES_KEY_TYPE type){
    switch (type) {
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
size_t aes_type_get_number_of_keys(AES_KEY_TYPE type){
    return type / 32;
}



// #------------------------------#
// | static functions definitions |
// #------------------------------#
static void get_key_expansion(const uint8_t* key_p,
                       const AES_KEY_TYPE type,
                       uint32_t* w_out){
    const size_t nk = aes_type_get_number_of_keys(type);
    uint32_t temp;
    size_t i = 0;
    while (i < nk){
        w_out[i] =  (uint32_t)key_p[4*i + 0] << 24 |
                    (uint32_t)key_p[4*i + 1] << 16 |
                    (uint32_t)key_p[4*i + 2] << 8 |
                    (uint32_t)key_p[4*i + 3];
        i = i+1;
    }
    i = nk;

    const size_t nr = aes_type_get_number_of_rounds(type);
    while (i < NB * (nr+1)) {
        temp = w_out[i-1];
        if (i % nk == 0) {
            temp = sub_word(rot_word(temp)) ^
                            get_round_constant(i/nk);
        }
        else if ((nk > 6) && (i % nk == 4)) {
            temp = sub_word(temp);
        }
        w_out[i] = w_out[i-nk] ^ temp;
        i = i + 1;
    }
}

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
