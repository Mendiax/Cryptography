#include <stdlib.h>
#include "test_utils.h"
#include "aes.h"
#include <assert.h>
#include <string.h>
#include <time.h>

// Function declarations
uint8_t *generate_random_data(size_t size);
void run_test_file(size_t file_size);

void test_run_big(void) {
    run_test_file(16 * 1000 * 1000);
}


// Function to generate random data of a specified size
uint8_t *generate_random_data(size_t size) {
    uint8_t *data = (uint8_t *)malloc(size);
    if (data == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < size; ++i) {
        data[i] = rand() % 256; // Generate random byte values (0-255)
    }

    return data;
}

// Function to run AES encryption and decryption test on a file of specified size
void run_test_file(size_t file_size) {
    // Generate random data for input file
    uint8_t *key = str_to_arr("000102030405060708090a0b0c0d0e0f");
    AES_KEY_TYPE type = AES_B128;
    Aes_Key *key_p = aes_new_key(type, key);
    assert(key_p);

    uint8_t *input = generate_random_data(file_size);
    uint8_t *input_copy = (uint8_t *)malloc(file_size);
    memcpy(input_copy, input, file_size);


    // Allocate memory for output buffer
    uint8_t *output = (uint8_t *)malloc(file_size);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Encrypt file and record time
    clock_t start_encrypt = clock();
    aes_encrypt_aligned(input, file_size, key_p, output);
    clock_t end_encrypt = clock();
    double encrypt_time = ((double)(end_encrypt - start_encrypt)) / CLOCKS_PER_SEC;

    // Decrypt file and record time
    clock_t start_decrypt = clock();
    aes_decrypt_aligned(output, file_size, key_p, input);
    clock_t end_decrypt = clock();
    double decrypt_time = ((double)(end_decrypt - start_decrypt)) / CLOCKS_PER_SEC;

    // Verify decrypted file matches original
    if(memcmp(input, input_copy, file_size) != 0){
        fprintf(stderr, "Encryption failed\n");
        exit(EXIT_FAILURE);
    }

    // Output benchmark results
    printf("File size: %zu bytes\n", file_size);
    printf("Encryption time: %.6f seconds\n", encrypt_time);
    printf("Decryption time: %.6f seconds\n", decrypt_time);

    // Cleanup
    aes_delete_key(&key_p);
    free(key);
    free(input);
    free(input_copy);
    free(output);
}
