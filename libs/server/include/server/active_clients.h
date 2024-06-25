#ifndef __SERVER_ACTIVE_CLIENTS_H__
#define __SERVER_ACTIVE_CLIENTS_H__
// #-------------------------------#
// |           includes            |
// #-------------------------------#
// c includes

// my includes
#include "server/config.h"
#include "server/msg.h"
#include "aes/aes.h"


// #-------------------------------#
// |            macros             |
// #-------------------------------#

// #-------------------------------#
// | global types declarations     |
// #-------------------------------#

typedef struct{
    SessionToken token;
    ConnectionState state;
    Aes_Key* key_p;
} ActiveClient;

// #-------------------------------#
// | global variables declarations |
// #-------------------------------#

// #-------------------------------#
// | global function declarations  |
// #-------------------------------#

ActiveClient* active_client_new();
void active_client_delete(ActiveClient** active_client_pp);


/**
 * @brief ad new client
 *
 * @return SessionToken
 */
SessionToken active_client_add();
/**
 * @brief remove client
 *
 * @param token
 */
void active_client_remove(SessionToken token);
/**
 * @brief active new client
 *
 * @param msg_p
 * @return ClientMsg*
 */
ClientMsg* active_client_handle_msg(const ServerMsg* msg_p);

/**
 * @brief Get client
 *
 * @param token
 * @return ActiveClient*
 */
ActiveClient* active_client_get(SessionToken token);



// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#

#endif
