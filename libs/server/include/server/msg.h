#ifndef __SERVER_MSG_H__
#define __SERVER_MSG_H__
// #-------------------------------#
// |           includes            |
// #-------------------------------#
// c includes
#include <stdint.h>

// my includes
#include "server/config.h"

// #-------------------------------#
// |            macros             |
// #-------------------------------#

// #-------------------------------#
// | global types declarations     |
// #-------------------------------#

typedef struct{
    uint16_t payload_size;
    char payload[];
}PlainMsg;

typedef struct{
    MessageDestination destination;
    SessionToken token;
    PlainMsg* msg_p;
}ServerMsg;

typedef struct{
    PlainMsg* msg_p;
}ClientMsg;

// #-------------------------------#
// | global variables declarations |
// #-------------------------------#

// #-------------------------------#
// | global function declarations  |
// #-------------------------------#
/**
 * @brief allocate new massage for given size and copy payload
 *
 * @param payload
 * @param payload_size
 * @return PlainMsg*
 */
PlainMsg *new_plain_msg(const char *payload, size_t payload_size);
/**
 * @brief allocate new message but do not copy palotad
 *
 * @param payload_size
 * @return PlainMsg*
 */
PlainMsg *new_plain_msg_empty(size_t payload_size);

size_t plain_msg_get_size(const PlainMsg * plain_msg_p);

void delete_plain_msg(PlainMsg** plain_msg_pp);


ServerMsg *new_server_msg(SessionToken session,
                                  MessageDestination destination,
                                  const char *payload, size_t payload_size);

void delete_server_msg(ServerMsg** server_msg_pp);

size_t server_msg_get_size(const ServerMsg * server_msg_p);



ClientMsg *new_client_msg(const void *payload, size_t payload_size);

void delete_client_msg(ClientMsg** client_msg_pp);


PlainMsg* plain_msg_serialize(const PlainMsg *msg);
PlainMsg* plain_msg_deserialize(const char *buffer);
PlainMsg* server_msg_serialize(const ServerMsg *msg);
ServerMsg* server_msg_deserialize(const char *buffer);
PlainMsg* client_msg_serialize(const ClientMsg *msg);
ClientMsg* client_msg_deserialize(const char *buffer);


// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#

#endif
