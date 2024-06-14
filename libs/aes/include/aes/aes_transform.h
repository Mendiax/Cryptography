#ifndef __CRYPTOGRAPHY_AES_TRANSFORM_H__
#define __CRYPTOGRAPHY_AES_TRANSFORM_H__
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

// #-------------------------------#
// | global variables declarations |
// #-------------------------------#

// #-------------------------------#
// | global function declarations  |
// #-------------------------------#
void aes_add_round_key(uint8_t *state, const uint32_t* w);

void mix_columns(uint8_t *state);
void shift_rows(uint8_t *state);
void sub_bytes(uint8_t *state);

void inv_mix_columns(uint8_t *state);
void inv_shift_rows(uint8_t *state);
void inv_sub_bytes(uint8_t *state);

// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#

#endif
