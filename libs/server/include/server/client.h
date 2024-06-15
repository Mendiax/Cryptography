#ifndef __SERVER_CLIENT_H__
#define __SERVER_CLIENT_H__
// #-------------------------------#
// |           includes            |
// #-------------------------------#
// c includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

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
int client_start(const char *server_ip, int port);
void client_stop(int client_fd);

// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#

#endif
