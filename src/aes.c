
// #------------------------------#
// |          includes            |
// #------------------------------#
// c includes
#include "aes.h"

// my includes
#include <assert.h>
#include <stdio.h>
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



static void aes_add_round_key(uint8_t *state, const uint32_t* w);
static uint8_t galois_mul(uint8_t a, uint8_t b);
static void mix_columns(uint8_t *state);
static void shift_rows(uint8_t *state);
static void sub_bytes(uint8_t *state);




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

// #------------------------------#
// | static functions definitions |
// #------------------------------#

static void aes_add_round_key(uint8_t *state_matrix, const uint32_t* w){
    for (int i = 0; i < 4; ++i) {
        uint8_t round_key_bytes[4];
        round_key_bytes[0] = (w[i] >> (24)) & 0xFF;
        round_key_bytes[1] = (w[i] >> (16)) & 0xFF;
        round_key_bytes[2] = (w[i] >> (8)) & 0xFF;
        round_key_bytes[3] = (w[i] >> (0)) & 0xFF;
        for (int j = 0; j < 4; ++j) {
            state_matrix[i * 4 + j] ^= round_key_bytes[j];
        }
    }
}


// Galois multiplication of two bytes
static uint8_t galois_mul(uint8_t a, uint8_t b) {
    uint8_t result = 0;
    uint8_t high_bit_set;
    for (int i = 0; i < 8; ++i) {
        if (b & 1) {
            result ^= a;
        }
        high_bit_set = a & 0x80;
        a <<= 1;
        if (high_bit_set) {
            a ^= 0x1B;  // XOR with the irreducible polynomial x^8 + x^4 + x^3 + x + 1
        }
        b >>= 1;
    }
    return result;
}

// MixColumns transformation
void mix_columns(uint8_t *state) {
    uint8_t tmp[4];
    for (int i = 0; i < 4; ++i) {
        // Perform MixColumns operation for each column of the state matrix

        // Calculate the column index in the state matrix
        int col_idx = i * 4;

        tmp[0] = galois_mul(state[col_idx], 2) ^ galois_mul(state[col_idx + 1], 3) ^ state[col_idx + 2] ^ state[col_idx + 3];
        tmp[1] = state[col_idx] ^ galois_mul(state[col_idx + 1], 2) ^ galois_mul(state[col_idx + 2], 3) ^ state[col_idx + 3];
        tmp[2] = state[col_idx] ^ state[col_idx + 1] ^ galois_mul(state[col_idx + 2], 2) ^ galois_mul(state[col_idx + 3], 3);
        tmp[3] = galois_mul(state[col_idx], 3) ^ state[col_idx + 1] ^ state[col_idx + 2] ^ galois_mul(state[col_idx + 3], 2);

        // Copy the result back to the state matrix
        for (int j = 0; j < 4; ++j) {
            state[col_idx + j] = tmp[j];
        }
    }
}

static void shift_rows(uint8_t *state){
    // No shift for the first row (row 0)

    // Shift the second row (row 1) one position to the left
    uint8_t tmp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = tmp;

    // Shift the third row (row 2) two positions to the left
    tmp = state[2];
    state[2] = state[10];
    state[10] = tmp;
    tmp = state[6];
    state[6] = state[14];
    state[14] = tmp;

    // Shift the fourth row (row 3) three positions to the left
    tmp = state[3];
    state[3] = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = tmp;
}

static void sub_bytes(uint8_t *state){
    static const uint8_t SBox[256] = {
        // S-box transformation table
        0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
        0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
        0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
        0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
        0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
        0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
        0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
        0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
        0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
        0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
        0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
        0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
        0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
        0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
        0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
        0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

    for (int i = 0; i < 16; ++i) {
        state[i] = SBox[state[i]];
    }
}
