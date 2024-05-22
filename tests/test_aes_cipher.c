#include "test_aes_cipher.h"
#include "aes.h"
#include "aes_transform.h"
#include "test_utils.h"

#include <assert.h>
#include <emmintrin.h>
#include <smmintrin.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_aes_cipher(void) {
  uint8_t key[128] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                      0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  AES_KEY_TYPE type = AES_B128;
  Aes_Key *key_p = aes_new_key(type, key);
  assert(key_p);

  uint8_t input[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
                     0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
  for (size_t idx = 0; idx < sizeof(input); idx++) {
    printf("%02x", input[idx]);
  }
  printf("\n");
  uint8_t output[16] = {0};
  uint8_t output_target[] = {0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb,
                             0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32};
  aes_encrypt_block(input, key_p, output);
  for (size_t idx = 0; idx < sizeof(output); idx++) {
    printf("%02x", output[idx]);
  }
  printf("\n");
  assert(memcmp(output, output_target, sizeof(output_target)) == 0);

  // cleanup
  aes_delete_key(&key_p);
}

static void print_m128i(__m128i resul_vec) {
  uint16_t elements[sizeof(__m128i) / 2];
  _mm_storeu_si128((__m128i *)elements, resul_vec);

  for (size_t i = 0; i < sizeof(elements) / sizeof(*elements); ++i) {
    printf("%04X ", elements[i]);
  }
  printf("\n");
}

void test_aes_transform_avx(void) {
  uint8_t state[] = {0xff, 0x0ff, 0xff, 0xff, 0xff, 0x0ff, 0xff, 0xff};
  uint8_t b_p[] =   {0b101, 0b101, 0x0, 0xff, 0b101, 0b101, 0x0, 0xff};
  unsigned loops = 1;
  for(int i = 0; i < 1; i++)
  {
    uint16_t a = state[i];
    uint8_t b = b_p[i];
    uint8_t result = 0;
    for (size_t i = 0; i < loops; ++i) {
        // if (b & 1) {
        //     result ^= a;
        // }
        a <<= 1;
        printf("%x\n", a);
        if (a & 0x100) {
            a ^= 0x1B;  // XOR with the irreducible polynomial x^8 + x^4 + x^3 + x + 1
        }
        printf("%x\n", a);
        b >>= 1;
    }
  }
  printf("\n");

  {
    __m128i a_vec = _mm_cvtepu8_epi16(_mm_loadu_si64((__m128i*)state));
    __m128i b_vec = _mm_cvtepu8_epi16(_mm_loadu_si64((__m128i*)b_p));

    print_m128i(a_vec);
    print_m128i(b_vec);

    __m128i resul_vec = _mm_setzero_si128();

    const __m128i mask_0x1 = _mm_set1_epi16(1);
    const __m128i mask_0x100 = _mm_set1_epi16(0x100);
    const __m128i mask_0x1B = _mm_set1_epi16(0x1B);
    printf("loop:\n");
    for (size_t i = 0; i < loops; ++i) {
      // const __m128i b_and_0x1_mask = _mm_and_si128(b_vec, mask_0x1);
      // const __m128i b_and_a_mask = _mm_mullo_epi16(b_and_0x1_mask, a_vec);
      // resul_vec = _mm_xor_si128(resul_vec, b_and_a_mask);
      // b_vec = _mm_srli_epi16(b_vec, 1);

      print_m128i(a_vec);
      a_vec = _mm_slli_epi16(a_vec, 1);
      print_m128i(a_vec);
      // a <<= 1;
      const __m128i a_and_0x100_mask = _mm_and_si128(a_vec, mask_0x100);
      const __m128i a_and_0x1_mask = _mm_srli_epi16(a_and_0x100_mask, 8);
      printf("a_and_0x100_mask:");print_m128i(a_and_0x100_mask);
      printf("a_and_0x1_mask:  ");print_m128i(a_and_0x1_mask);
      const __m128i a_and_a_mask = _mm_mullo_epi16(a_and_0x1_mask, mask_0x1B);
      printf("mask_0x1B:       ");print_m128i(mask_0x1B);

      printf("a_and_a_mask:    ");print_m128i(a_and_a_mask);
      a_vec = _mm_xor_si128(a_vec, a_and_a_mask);
      print_m128i(a_vec);


    }

    // assert(0);
  }
}

void test_aes_transform(void) {
  uint8_t state[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
                     0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

  uint8_t state_copy[16];
  memcpy(state_copy, state, sizeof(state));

  shift_rows(state_copy);
  inv_shift_rows(state_copy);

  if (memcmp(state, state_copy, sizeof(state)) != 0) {
    for (size_t idx = 0; idx < sizeof(state); idx++) {
      printf("%02x ", state[idx]);
    }
    printf("\n");
    for (size_t idx = 0; idx < sizeof(state_copy); idx++) {
      printf("%02x ", state_copy[idx]);
    }
    printf("\n");
    assert(memcmp(state, state_copy, sizeof(state)) == 0);
  }

  memcpy(state_copy, state, sizeof(state));
  mix_columns(state_copy);
  inv_mix_columns(state_copy);
  if (memcmp(state, state_copy, sizeof(state)) != 0) {
    for (size_t idx = 0; idx < sizeof(state); idx++) {
      printf("%02x ", state[idx]);
    }
    printf("\n");
    for (size_t idx = 0; idx < sizeof(state_copy); idx++) {
      printf("%02x ", state_copy[idx]);
    }
    printf("\n");
    assert(memcmp(state, state_copy, sizeof(state)) == 0);
  }

  memcpy(state_copy, state, sizeof(state));
  sub_bytes(state_copy);
  inv_sub_bytes(state_copy);
  if (memcmp(state, state_copy, sizeof(state)) != 0) {
    for (size_t idx = 0; idx < sizeof(state); idx++) {
      printf("%02x ", state[idx]);
    }
    printf("\n");
    for (size_t idx = 0; idx < sizeof(state_copy); idx++) {
      printf("%02x ", state_copy[idx]);
    }
    printf("\n");
    assert(memcmp(state, state_copy, sizeof(state)) == 0);
  }
}

void test_aes_inv_cipher(void) {
  uint8_t key[128] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                      0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  AES_KEY_TYPE type = AES_B128;
  Aes_Key *key_p = aes_new_key(type, key);
  assert(key_p);

  uint8_t input[] = {0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb,
                     0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32};

  uint8_t output[16] = {0};
  uint8_t output_target[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
                             0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
  aes_decrypt_block(input, key_p, output);
  for (size_t idx = 0; idx < sizeof(output); idx++) {
    printf("%02x", output[idx]);
  }
  printf("\n");
  assert(memcmp(output, output_target, sizeof(output_target)) == 0);

  // cleanup
  aes_delete_key(&key_p);
}

void test_aes_2_way_128(void) {
  uint8_t *key = str_to_arr("000102030405060708090a0b0c0d0e0f");
  AES_KEY_TYPE type = AES_B128;
  Aes_Key *key_p = aes_new_key(type, key);
  assert(key_p);

  uint8_t *input = str_to_arr("00112233445566778899aabbccddeeff");

  uint8_t output[16] = {0};
  uint8_t *output_target = str_to_arr("69c4e0d86a7b0430d8cdb78070b4c55a");

  aes_encrypt_block(input, key_p, output);
  for (size_t idx = 0; idx < sizeof(output); idx++) {
    printf("%02x", output[idx]);
  }
  printf("\n");
  assert(memcmp(output, output_target, sizeof(output)) == 0);
  printf("Encrypt ok\n");

  // swap
  uint8_t *temp = input;
  input = output_target;
  output_target = temp;

  aes_decrypt_block(input, key_p, output);
  for (size_t idx = 0; idx < sizeof(output); idx++) {
    printf("%02x", output[idx]);
  }
  printf("\n");
  assert(memcmp(output, output_target, sizeof(output)) == 0);

  // cleanup
  aes_delete_key(&key_p);
  free(key);
  free(input);
  free(output_target);
}

void test_aes_2_way_192(void) {
  uint8_t *key = str_to_arr("000102030405060708090a0b0c0d0e0f1011121314151617");
  AES_KEY_TYPE type = AES_B192;
  Aes_Key *key_p = aes_new_key(type, key);
  assert(key_p);

  uint8_t *input = str_to_arr("00112233445566778899aabbccddeeff");

  uint8_t output[16] = {0};
  uint8_t *output_target = str_to_arr("dda97ca4864cdfe06eaf70a0ec0d7191");

  aes_encrypt_block(input, key_p, output);
  for (size_t idx = 0; idx < sizeof(output); idx++) {
    printf("%02x", output[idx]);
  }
  printf("\n");
  assert(memcmp(output, output_target, sizeof(output)) == 0);
  printf("Encrypt ok\n");

  // swap
  uint8_t *temp = input;
  input = output_target;
  output_target = temp;
  aes_decrypt_block(input, key_p, output);
  for (size_t idx = 0; idx < sizeof(output); idx++) {
    printf("%02x", output[idx]);
  }
  printf("\n");
  assert(memcmp(output, output_target, sizeof(output)) == 0);

  // cleanup
  aes_delete_key(&key_p);
  free(key);
  free(input);
  free(output_target);
}
void test_aes_2_way_256(void) {
  uint8_t *key = str_to_arr(
      "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f");
  AES_KEY_TYPE type = AES_B256;
  Aes_Key *key_p = aes_new_key(type, key);
  assert(key_p);

  uint8_t *input = str_to_arr("00112233445566778899aabbccddeeff");

  uint8_t output[16] = {0};
  uint8_t *output_target = str_to_arr("8ea2b7ca516745bfeafc49904b496089");

  aes_encrypt_block(input, key_p, output);
  for (size_t idx = 0; idx < sizeof(output); idx++) {
    printf("%02x", output[idx]);
  }
  printf("\n");
  assert(memcmp(output, output_target, sizeof(output)) == 0);
  printf("Encrypt ok\n");

  // swap
  uint8_t *temp = input;
  input = output_target;
  output_target = temp;
  aes_decrypt_block(input, key_p, output);
  for (size_t idx = 0; idx < sizeof(output); idx++) {
    printf("%02x", output[idx]);
  }
  printf("\n");
  assert(memcmp(output, output_target, sizeof(output)) == 0);

  // cleanup
  aes_delete_key(&key_p);
  free(key);
  free(input);
  free(output_target);
}
