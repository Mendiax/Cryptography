#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline uint8_t *str_to_arr(const char *hex_string) {
    size_t len = strlen(hex_string);
    if (len % 2 != 0) {
        // Hex string length must be even
        return NULL;
    }

    size_t array_len = len / 2;
    uint8_t *array = (uint8_t *)malloc(array_len * sizeof(uint8_t));
    if (array == NULL) {
        // Memory allocation failed
        return NULL;
    }

    for (size_t i = 0; i < array_len; ++i) {
        // Convert each pair of hexadecimal characters to uint8_t
        sscanf(&hex_string[i * 2], "%2hhx", &array[i]);
    }

    return array;
}