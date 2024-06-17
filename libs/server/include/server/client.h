#ifndef __SERVER_CLIENT_H__
#define __SERVER_CLIENT_H__
// #-------------------------------#
// |           includes            |
// #-------------------------------#
// c includes
#include <unistd.h>
#include "server/msg.h"

// my includes

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
PlainMsg* client_send_msg(int client_fd, ServerMsg* msg_p);
int client_start(const char *server_ip, int port);
void client_stop(int client_fd);

// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#

#endif
