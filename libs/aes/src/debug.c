
// #------------------------------#
// |          includes            |
// #------------------------------#
// c includes
#include <stdio.h>

// my includes
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
void print_state(uint8_t* state){
    printf("%02x %02x %02x %02x\n", state[0], state[4], state[8], state[12]);
    printf("%02x %02x %02x %02x\n", state[1], state[5], state[9], state[13]);
    printf("%02x %02x %02x %02x\n", state[2], state[6], state[10], state[14]);
    printf("%02x %02x %02x %02x\n", state[3], state[7], state[11], state[15]);
    printf("\n\n");
}

void print_state_line(uint8_t* state){
    printf("%02x%02x%02x%02x", state[0], state[1], state[2], state[3]);
    printf("%02x%02x%02x%02x", state[4], state[5], state[6], state[7]);
    printf("%02x%02x%02x%02x", state[8], state[9], state[10], state[11]);
    printf("%02x%02x%02x%02x", state[12], state[13], state[14], state[15]);
    printf("\n\n");
}


// #------------------------------#
// | static functions definitions |
// #------------------------------#
