
// #------------------------------#
// |          includes            |
// #------------------------------#
// c includes
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

// my includes
#include "server/broadcast.h"
#include "server/ake.h"
#include "server/utils.h"
#include "server/active_clients.h"


// #------------------------------#
// |           macros             |
// #------------------------------#

// #------------------------------#
// | global variables definitions |
// #------------------------------#

// #------------------------------#
// | static variables definitions |
// #------------------------------#

// #------------------------------#
// | static functions declarations|
// #------------------------------#

// #------------------------------#
// | global function definitions  |
// #------------------------------#

// #------------------------------#
// | static functions definitions |
// #------------------------------#

void* broadcast_msg_handler(void* arg) {
    int client_socket = *((int *)arg);

    while (1) {
        printf("Got new message\n");
        char* buffer;
        uint16_t size;
        if(read_with_size_prefix(client_socket, &buffer, &size)) {
            perror("Error reading from socket");
            goto EXIT_HANDLER;
        }

        // Read client request
        ServerMsg* msg_to_server = server_msg_deserialize(buffer);
        free(buffer);
        // Ger response
        ClientMsg* client_msg = broadcast_msg(msg_to_server);
        delete_server_msg(&msg_to_server);


        if(client_msg){
            printf("Sending response\n");
            if (write_with_size_prefix(client_socket, client_msg->msg_p->payload, client_msg->msg_p->payload_size)) {
                perror("Error writing to socket");
                goto EXIT_HANDLER;
            }
        }
        delete_client_msg(&client_msg);
    }
EXIT_HANDLER:
    // Close client socket
    close(client_socket);
    pthread_exit(NULL);
}

ClientMsg* broadcast_msg(const ServerMsg* msg_p)
{
    switch (msg_p->destination) {
    case MESSAGE_DESTINATION_NONE:
        return NULL;
    case MESSAGE_DESTINATION_AKE:
        return ake_handle_msg(msg_p);
    case MESSAGE_DESTINATION_HANDLER:
      break;
    case MESSAGE_DESTINATION_INIT:
        return active_client_handle_msg(msg_p);
      break;
    }
    return NULL;
}
