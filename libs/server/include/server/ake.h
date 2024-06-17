#ifndef __SERVER_AKE_H__
#define __SERVER_AKE_H__
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

ClientMsg* ake_handle_msg(const ServerMsg* msg_p);

// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#

#endif
