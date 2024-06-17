
// #------------------------------#
// |          includes            |
// #------------------------------#
// c includes
#include <stdio.h>
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

static ActiveClient active_clients[MAX_CONNECTIONS];
static size_t no_active_clients = 0;

// #------------------------------#
// | static functions declarations|
// #------------------------------#

inline static SessionToken get_new_token();

// #------------------------------#
// | global function definitions  |
// #------------------------------#

SessionToken active_client_add() {
    if(no_active_clients < MAX_CONNECTIONS) {
        const SessionToken new_token = get_new_token();
        active_clients[no_active_clients].token = new_token;
        return new_token;
    }
    return SESSION_TOKEN_NONE;
}
void active_client_remove(SessionToken token) {
    for(size_t i =0; i < no_active_clients; i++ ){
        if(active_clients[i].token == token){
            memmove(&active_clients[i], &active_clients[i+1], no_active_clients - 1);
            active_clients[no_active_clients] = (ActiveClient){0};
            no_active_clients--;
            return;
        }
    }
}

ClientMsg* active_client_handle_msg(const ServerMsg* msg_p)
{
    (void) msg_p;
    SessionToken new_token = active_client_add();
    printf("session: %zu\n", new_token);
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
