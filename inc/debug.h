#ifndef __CRYPTOGRAPHY_DEBUG_H__
#define __CRYPTOGRAPHY_DEBUG_H__
// #-------------------------------#
// |           includes            |
// #-------------------------------#
// c includes
#include <stdint.h>
// my includes

// #-------------------------------#
// |            macros             |
// #-------------------------------#

#ifndef NDEBUG
#define DEBUG_STATE(X) print_state_line(X)
#else
#define DEBUG_STATE(X)
#endif

// #-------------------------------#
// | global types declarations     |
// #-------------------------------#

// #-------------------------------#
// | global variables declarations |
// #-------------------------------#

// #-------------------------------#
// | global function declarations  |
// #-------------------------------#

void print_state(uint8_t* state);
void print_state_line(uint8_t* state);

// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#

#endif
