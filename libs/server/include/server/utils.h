#ifndef __SERVER_UTILS_H__
#define __SERVER_UTILS_H__
// #-------------------------------#
// |           includes            |
// #-------------------------------#
// c includes
#include <arpa/inet.h>
// my includes
#include "aes/aes.h"

// #-------------------------------#
// |            macros             |
// #-------------------------------#
#if __BIG_ENDIAN__
# define htonll(x) (x)
# define ntohll(x) (x)
#else
# define htonll(x) (((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
# define ntohll(x) (((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))
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

int read_with_size_prefix(int fd, char **data, uint16_t *size, const Aes_Key* key);
int write_with_size_prefix(int fd, const char *data, uint16_t size, const Aes_Key* key);

// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#

#endif
