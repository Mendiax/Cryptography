#ifndef __SERVER_SERVER_H__
#define __SERVER_SERVER_H__
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
#define BUFFER_SIZE 1024

// #-------------------------------#
// | global types declarations     |
// #-------------------------------#

// #-------------------------------#
// | global variables declarations |
// #-------------------------------#

// #-------------------------------#
// | global function declarations  |
// #-------------------------------#


typedef void* (*ServerRequestHandler)(void*);

void server_start(int port, ServerRequestHandler request_handler);



// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#

#endif
