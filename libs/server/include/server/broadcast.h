#ifndef __SERVER_BROADCAST_H__
#define __SERVER_BROADCAST_H__
// #-------------------------------#
// |           includes            |
// #-------------------------------#
// c includes

// my includes
#include "server/msg.h"

// #-------------------------------#
// |            macros             |
// #-------------------------------#

// #-------------------------------#
// | global types declarations     |
// #-------------------------------#

// #-------------------------------#
// | global variables declarations |
// #-------------------------------#

// #-------------------------------#
// | global function declarations  |
// #-------------------------------#

void* broadcast_msg_handler(void*);

ClientMsg* broadcast_msg(const ServerMsg* msg_p);

// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#

#endif
