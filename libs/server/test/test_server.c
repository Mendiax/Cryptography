#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

#include "dh/dh.h"
#include "aes/aes.h"
#include "server/utils.h"
#include "server/client.h"
#include "server/server.h"
#include "server/broadcast.h"


void* server_thread(void* args);
#define PORT 44444
#define LOCAL_HOST "127.0.0.1"


int main() {
    OpenSSL_add_all_algorithms();
    pthread_t server;
    pthread_create(&server, NULL, server_thread, NULL);

    // Run as client
    int client_fd = client_start(LOCAL_HOST, PORT);

    // get session token
    int success = EXIT_SUCCESS;
    SessionToken session = SESSION_TOKEN_NONE;
    Aes_Key* key_p = NULL;
    {
        ServerMsg *msg =
            new_server_msg(session, MESSAGE_DESTINATION_INIT, "HELLO",
                            sizeof("HELLO"));
        printf("Writing messages\n");
        PlainMsg *response = client_send_msg(client_fd, msg);
        delete_server_msg(&msg);
        printf("Received message\n");
        // handle message
        session = ntohll(*(SessionToken *)response->payload);
        printf("Token: %lu\n", session);
        delete_plain_msg(&response);

        if (session != 1) {
            success = EXIT_FAILURE;
        }
    }
    {
        EVP_PKEY* client_key = generate_key(CURVE_NAME);
        char* client_public_key = get_public_key_hex(client_key);


        ServerMsg *msg =
        new_server_msg(session, MESSAGE_DESTINATION_AKE, client_public_key,
                        strlen(client_public_key) + 1);
        printf("[CLIENT] Writing messages\n");
        PlainMsg *response = client_send_msg(client_fd, msg);
        delete_server_msg(&msg);
        printf("[CLIENT] Received message\n");

        // handle message
        const char* server_public_key = response->payload;
        printf("[CLIENT] Server PK: %s\n", server_public_key);
        char* shared_secret = derive_shared_secret(client_key, server_public_key);
        printf("[CLIENT] Secret: %s\n", shared_secret);

        unsigned char* shared_secret_bin = convert_hex_str_to_bytes(shared_secret);
        const size_t shared_secret_len = strlen(shared_secret) / 2;

        unsigned char* aes_key_128 = derive_aes_key(shared_secret_bin, shared_secret_len, AES_B128);
        key_p = aes_new_key(AES_B128, aes_key_128);

        delete_plain_msg(&response);
        free(client_public_key);
        OPENSSL_free(client_key);
    }
    printf("[TEST] test encrypted\n");
    {
        const char* msg_payload= "Hello";
        ServerMsg *msg =
            new_server_msg(session, MESSAGE_DESTINATION_HANDLER, msg_payload,
                            strlen(msg_payload));
        printf("[CLIENT] Writing messages\n");
        PlainMsg *response = client_send_msg_enc(client_fd, msg, key_p);
        delete_server_msg(&msg);
        printf("[CLIENT] Received message\n");
        // handle message
        printf("[CLIENT] Client got %s\n", response->payload);
        assert(strcmp(msg_payload, response->payload) == 0);

        delete_plain_msg(&response);

        if (session != 1) {
            success = EXIT_FAILURE;
        }
    }

    client_stop(client_fd);
    pthread_cancel(server);

    return success;
}


void* server_thread(void* args) {
    (void) args;
    server_start(PORT, broadcast_msg_handler);
    return 0;
}