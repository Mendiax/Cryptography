
// #------------------------------#
// |          includes            |
// #------------------------------#
// c includes
#include <assert.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>

// my includes
#include "server/utils.h"

// #------------------------------#
// |           macros             |
// #------------------------------#

// #define DEBUG(x) x
#define DEBUG(x, ...)

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

/*
Packet:
|size (2bytes)| payload (size bytes)|
*/

int read_with_size_prefix(int fd, char **data, uint16_t *size, const Aes_Key* key) {
   uint16_t size_be;

    // First read the size
    DEBUG("[read] Reading message size\n");
    ssize_t read_bytes = read(fd, &size_be, sizeof(uint16_t));
    if (read_bytes != sizeof(uint16_t)) {
        return -1; // Error in reading size
    }
    const uint16_t buffer_size = ntohs(size_be);
    DEBUG("[read] Read %zd Bytes\n", read_bytes);
    DEBUG("[read] Got %u Bytes message size\n", buffer_size);


    assert(buffer_size > 0);
    char* buffer = malloc(buffer_size);
    if (buffer == NULL) {
        return -1; // Memory allocation error
    }
    DEBUG("[read] Reading message\n");
    // Then read the actual data
    read_bytes = read(fd, buffer, buffer_size);
    if (read_bytes != buffer_size) {
        free(buffer);
        buffer = NULL;
        return -1; // Error in reading data
    }
    DEBUG("[read] Read %zd\n", read_bytes);
    if (key) {
        aes_decrypt_aligned((uint8_t*) buffer,  read_bytes, key, (uint8_t*) buffer);
    }

    *data = buffer;
    *size = buffer_size;
    return 0; // Success
}

int write_with_size_prefix(int fd, const char *data, uint16_t size, const Aes_Key* key) {

    char* buffer;
    if (key) {
        // printf("Align length %u", size);
        size = AES_ALIGN_LENGTH(size);
        // printf("-> %u\n", size);

        buffer = malloc(size);
        aes_encrypt_aligned((uint8_t*) data,  size, key, (uint8_t*) buffer);
    }
    const char* send_buffer = key == NULL ? data : buffer;

    const uint16_t size_be = htons(size);

    // First write the size
    DEBUG("[write] Writing message size %u\n", size);
    ssize_t written = write(fd, &size_be, sizeof(size_be));
    if (written != sizeof(size_be)) {
        return -1; // Error in writing size
    }
    DEBUG("[write] Writing message\n");
    // Then write the actual data
    written = write(fd, send_buffer, size);
    if (written != size) {
        return -1; // Error in writing data
    }
    if (key) {
        free(buffer);
    }

    return 0; // Success
}


// #------------------------------#
// | static functions definitions |
// #------------------------------#
