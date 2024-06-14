
// #------------------------------#
// |          includes            |
// #------------------------------#
// c includes
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// my includes
#include "aes/aes.h"
#include "aes/aes_transform.h"
#include "aes/debug.h"

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

void aes_encrypt_block(const uint8_t *input, const Aes_Key *key_p, uint8_t *output){
    assert(key_p);
    const size_t nr = aes_type_get_number_of_rounds(key_p->type);

    uint8_t state[4 * NB];
    memcpy(state, input, sizeof(state));

    aes_add_round_key(state, key_p->key);

    for (size_t round = 1; round < nr; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        aes_add_round_key(state, &key_p->key[round*NB]);
    }

    sub_bytes(state);
    shift_rows(state);
    aes_add_round_key(state, &key_p->key[nr*NB]);

    memcpy(output, state, sizeof(state));
}

void aes_decrypt_block(const uint8_t *input, const Aes_Key *key_p, uint8_t *output){
    assert(key_p);
    const size_t nr = aes_type_get_number_of_rounds(key_p->type);

    uint8_t state[4 * NB];
    memcpy(state, input, sizeof(state));

    aes_add_round_key(state, &key_p->key[nr*NB]);

    for (size_t round = nr - 1; round >= 1; round--) {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        aes_add_round_key(state, &key_p->key[round*NB]);
        inv_mix_columns(state);
    }

    inv_shift_rows(state);
    inv_sub_bytes(state);
    aes_add_round_key(state, key_p->key);

    memcpy(output, state, sizeof(state));
}

void aes_encrypt_aligned(const uint8_t *input, size_t input_len, const Aes_Key *key_p, uint8_t *output) {
    // Calculate number of blocks
    assert(input_len % (128/8) == 0);
    size_t num_blocks = input_len / 16; // Round up to nearest multiple of 16 bytes

    // Process each block
    for (size_t i = 0; i < num_blocks; ++i) {
        // Calculate start and end indices for this block
        size_t start_index = i * 16;
        size_t end_index = (i + 1) * 16;
        if (end_index > input_len) {
            end_index = input_len;
        }

        // Encrypt block
        aes_encrypt_block(input + start_index, key_p, output + start_index);
    }
}

void aes_decrypt_aligned(const uint8_t *input, size_t input_len, const Aes_Key *key_p, uint8_t *output) {
    // Calculate number of blocks
    assert(input_len % (128/8) == 0);

    size_t num_blocks = input_len / 16; // Round up to nearest multiple of 16 bytes

    // Process each block
    for (size_t i = 0; i < num_blocks; ++i) {
        // Calculate start and end indices for this block
        size_t start_index = i * 16;
        size_t end_index = (i + 1) * 16;
        if (end_index > input_len) {
            end_index = input_len;
        }

        // Decrypt block
        aes_decrypt_block(input + start_index, key_p, output + start_index);
    }
}

// #------------------------------#
// | static functions definitions |
// #------------------------------#


