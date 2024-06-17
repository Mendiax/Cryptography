#ifndef __SERVER_CONFIG_H__
#define __SERVER_CONFIG_H__
// #-------------------------------#
// |           includes            |
// #-------------------------------#
// c includes
#include <stdint.h>

// my includes

// #-------------------------------#
// |            macros             |
// #-------------------------------#

#define MAX_CONNECTIONS 10
#define SESSION_TOKEN_NONE 0

// #-------------------------------#
// | global types declarations     |
// #-------------------------------#
typedef enum {
    MESSAGE_DESTINATION_NONE,
    MESSAGE_DESTINATION_INIT,
    MESSAGE_DESTINATION_AKE,
    MESSAGE_DESTINATION_HANDLER
} MessageDestination;

typedef enum  {
    CONNECTION_NONE,
    CONNECTION_DISCONNECTED,
    CONNECTION_CONNECTED,
    CONNECTION_KEY_EXCHANGE,
    CONNECTION_AUTHENTICATED
} ConnectionState;

typedef uint64_t SessionToken;

// #-------------------------------#
// | global variables declarations |
// #-------------------------------#

// #-------------------------------#
// | global function declarations  |
// #-------------------------------#

// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#

#endif
