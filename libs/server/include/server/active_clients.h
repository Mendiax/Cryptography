#ifndef __SERVER_ACTIVE_CLIENTS_H__
#define __SERVER_ACTIVE_CLIENTS_H__
// #-------------------------------#
// |           includes            |
// #-------------------------------#
// c includes

// my includes
#include "server/config.h"
#include "server/msg.h"

// #-------------------------------#
// |            macros             |
// #-------------------------------#

// #-------------------------------#
// | global types declarations     |
// #-------------------------------#

typedef struct{
    SessionToken token;
    ConnectionState state;
} ActiveClient;

// #-------------------------------#
// | global variables declarations |
// #-------------------------------#

// #-------------------------------#
// | global function declarations  |
// #-------------------------------#

SessionToken active_client_add();
void active_client_remove(SessionToken token);
ClientMsg* active_client_handle_msg(const ServerMsg* msg_p);

// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#

#endif
