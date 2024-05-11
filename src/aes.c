
// #------------------------------#
// |          includes            |
// #------------------------------#
// c includes
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// my includes
#include "aes.h"
#include "aes_transform.h"
#include "debug.h"

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
    const size_t nr = get_number_of_rounds(key_p);
    uint32_t* w = malloc(4*(nr+1) * sizeof(uint32_t));

    get_key_expansion(key_p, w);

    uint8_t state[4 * 4];
    memcpy(state, input, sizeof(state));

    aes_add_round_key(state, w);

    for (size_t round = 1; round < nr; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        aes_add_round_key(state, &w[round*4]);
    }

    sub_bytes(state);
    shift_rows(state);
    aes_add_round_key(state, &w[nr*4]);

    memcpy(output, state, sizeof(state));
}

void aes_decrypt_block(const uint8_t *input, const Aes_Key *key_p, uint8_t *output){
    assert(key_p);
    const size_t nr = get_number_of_rounds(key_p);
    uint32_t* w = malloc(4*(nr+1) * sizeof(uint32_t));

    get_key_expansion(key_p, w);

    uint8_t state[4 * 4];
    memcpy(state, input, sizeof(state));

    DEBUG_STATE(state);
    aes_add_round_key(state, &w[nr*4]);

    for (size_t round = nr - 1; round >= 1; round--) {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        aes_add_round_key(state, &w[round*4]);
        inv_mix_columns(state);
    }
    DEBUG_STATE(state);

    inv_shift_rows(state);
    inv_sub_bytes(state);
    aes_add_round_key(state, w);

    memcpy(output, state, sizeof(state));
}

// #------------------------------#
// | static functions definitions |
// #------------------------------#


