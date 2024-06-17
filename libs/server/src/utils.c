
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

int read_with_size_prefix(int fd, char **data, uint16_t *size) {
   uint16_t size_be;

    // First read the size
    printf("[read] Reading message size\n");
    ssize_t read_bytes = read(fd, &size_be, sizeof(uint16_t));
    if (read_bytes != sizeof(uint16_t)) {
        return -1; // Error in reading size
    }
    const uint16_t buffer_size = ntohs(size_be);
    printf("[read] Read %zd Bytes\n", read_bytes);
    printf("[read] Got %u Bytes message size\n", buffer_size);


    assert(buffer_size > 0);
    char* buffer = malloc(buffer_size);
    if (buffer == NULL) {
        return -1; // Memory allocation error
    }
    printf("[read] Reading message\n");
    // Then read the actual data
    read_bytes = read(fd, buffer, buffer_size);
    if (read_bytes != buffer_size) {
        free(buffer);
        buffer = NULL;
        return -1; // Error in reading data
    }
    printf("[read] Read %zd\n", read_bytes);

    *data = buffer;
    *size = buffer_size;
    return 0; // Success
}

int write_with_size_prefix(int fd, const char *data, uint16_t size) {
    const uint16_t size_be = htons(size);

    // First write the size
    printf("[write] Writing message size %u\n", size);
    ssize_t written = write(fd, &size_be, sizeof(size_be));
    if (written != sizeof(size_be)) {
        return -1; // Error in writing size
    }
    printf("[write] Writing message\n");
    // Then write the actual data
    written = write(fd, data, size);
    if (written != size) {
        return -1; // Error in writing data
    }

    return 0; // Success
}


// #------------------------------#
// | static functions definitions |
// #------------------------------#
