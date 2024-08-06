
// #------------------------------#
// |          includes            |
// #------------------------------#
// c includes
#include <stdio.h>

// my includes
#include "server/active_clients.h"
#include "server/ake.h"
#include "dh/dh.h"

// #------------------------------#
// |           macros             |
// #------------------------------#

// #------------------------------#
// | global variables definitions |
// #------------------------------#

// #------------------------------#
// | static variables definitions |
// #------------------------------#

static EVP_PKEY* server_key = NULL;

// #------------------------------#
// | static functions declarations|
// #------------------------------#

// #------------------------------#
// | global function definitions  |
// #------------------------------#

ClientMsg* ake_handle_msg(const ServerMsg* msg_p)
{
    printf("[SERVER] session: %zu\n", msg_p->token);
    ActiveClient* client_p = active_client_get(msg_p->token);
    if(client_p == NULL) {
        printf("[SERVER] Invalid session %zu\n", msg_p->token);
        return NULL;
    }
    if(server_key == NULL) {
        server_key = generate_key(CURVE_NAME);
    }
    const char* client_public_key = msg_p->msg_p->payload;
    char* server_shared_secret = derive_shared_secret(server_key, client_public_key);
    unsigned char* shared_secret_bin = convert_hex_str_to_bytes(server_shared_secret);
    const size_t shared_secret_len = strlen(server_shared_secret) / 2;
    printf("[SERVER] Secret: %s\n", server_shared_secret);

    unsigned char* aes_key_128 = derive_aes_key(shared_secret_bin, shared_secret_len, AES_B128);
    client_p->key_p = aes_new_key(AES_B128, aes_key_128);
    free(shared_secret_bin);
    free(server_shared_secret);
    // free(aes_key_128);

    // response with server pk
    char* server_public_key = get_public_key_hex(server_key);
    ClientMsg* response = new_client_msg(server_public_key, strlen(server_public_key) + 1);
    return response;
}

// #------------------------------#
// | static functions definitions |
// #------------------------------#
