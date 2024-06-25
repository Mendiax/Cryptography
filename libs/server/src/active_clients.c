
// #------------------------------#
// |          includes            |
// #------------------------------#
// c includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// my includes
#include "server/active_clients.h"
#include "server/msg.h"
#include "server/utils.h"


// #------------------------------#
// |           macros             |
// #------------------------------#

// #------------------------------#
// | global variables definitions |
// #------------------------------#

// #------------------------------#
// | static variables definitions |
// #------------------------------#

static ActiveClient* active_clients[MAX_CONNECTIONS];
static size_t no_active_clients = 0;

// #------------------------------#
// | static functions declarations|
// #------------------------------#

inline static SessionToken get_new_token();

// #------------------------------#
// | global function definitions  |
// #------------------------------#

ActiveClient* active_client_new() {
    ActiveClient* new_client = malloc(sizeof(ActiveClient));
    new_client->key_p = NULL;
    new_client->token = SESSION_TOKEN_NONE;
    new_client->state = CONNECTION_NONE;
    return new_client;
}
void active_client_delete(ActiveClient** active_client_pp) {
    free(*active_client_pp);
}


ActiveClient* active_client_get(SessionToken token) {
    for(size_t i = 0; i < no_active_clients; i++ ){
        // printf("%p\t", active_clients[i]);
        if(active_clients[i] && active_clients[i]->token == token){
            return active_clients[i];
        }
    }
    // printf("\n");
    return NULL;
}

SessionToken active_client_add() {
    if(no_active_clients < MAX_CONNECTIONS) {
        const SessionToken new_token = get_new_token();
        active_clients[no_active_clients] = active_client_new();
        active_clients[no_active_clients]->token = new_token;
        no_active_clients++;
        return new_token;
    }

    return SESSION_TOKEN_NONE;
}
void active_client_remove(SessionToken token) {
    for(size_t i =0; i < no_active_clients; i++ ){
        if(active_clients[i] && active_clients[i]->token == token){
            active_client_delete(&active_clients[i]);
            memmove(&active_clients[i], &active_clients[i+1], no_active_clients - 1);
            active_clients[no_active_clients] = NULL;
            no_active_clients--;
            return;
        }
    }
}

ClientMsg* active_client_handle_msg_refresh(const ServerMsg* msg_p)
{
    (void) msg_p;
    ActiveClient* client_p = active_client_get(msg_p->token);
    if (client_p == NULL) {
        const SessionToken new_token = SESSION_TOKEN_NONE;
        return new_client_msg((char*)&new_token, sizeof(SESSION_TOKEN_NONE));
    }
    const SessionToken new_token = get_new_token();
    printf("session: %zu\n", new_token);
    client_p->token = new_token;

    uint64_t token_payload = htonll(new_token);
    return new_client_msg((char*)&token_payload, sizeof(new_token));
}

ClientMsg* active_client_handle_msg(const ServerMsg* msg_p)
{
    (void) msg_p;
    SessionToken new_token = active_client_add();
    printf("New session: %zu\n", new_token);
    uint64_t token_payload = htonll(new_token);
    return new_client_msg((char*)&token_payload, sizeof(new_token));
}

// #------------------------------#
// | static functions definitions |
// #------------------------------#

inline static SessionToken get_new_token()
{
    static SessionToken token = 1;
    return token++;
}
