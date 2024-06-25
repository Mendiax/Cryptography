#ifndef __SERVER_CLIENT_H__
#define __SERVER_CLIENT_H__
// #-------------------------------#
// |           includes            |
// #-------------------------------#
// c includes
#include <unistd.h>

// my includes
#include "server/msg.h"
#include "aes/aes.h"

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
PlainMsg* client_send_msg_enc(int client_fd, ServerMsg* msg_p, const Aes_Key* key);
int client_start(const char *server_ip, int port);
void client_stop(int client_fd);

// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#

#endif
