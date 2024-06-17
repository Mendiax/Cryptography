#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "server/client.h"
#include "server/server.h"
#include "server/broadcast.h"
#include "server/utils.h"




void print_usage() {
    printf("Usage:\n");
    printf("  To run as server: ./main server <port>\n");
    printf("  To run as client: ./main client <server_ip> <port>\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage();
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "server") == 0) {
        // Run as server
        int port = atoi(argv[2]);
        server_start(port, broadcast_msg_handler);
    } else if (strcmp(argv[1], "client") == 0) {
        // Run as client
        const char *server_ip = argv[2];
        int port = atoi(argv[3]);
        int client_fd = client_start(server_ip, port);

        ServerMsg* msg = new_server_msg(SESSION_TOKEN_NONE, MESSAGE_DESTINATION_INIT, "HELLO", sizeof("HELLO"));
        printf("Writing messages\n");
        PlainMsg* response = client_send_msg(client_fd, msg);
        delete_server_msg(&msg);
        printf("Received message\n");
        // handle message
        SessionToken session =  ntohll(*(SessionToken*)response->payload);
        printf("Token: %lu\n", session);
        delete_plain_msg(&response);

        // // Example client interaction: send a message and receive response
        // char* message = NULL;
        // while (1) { // ^D
        //     scanf("%ms", &message);
        //     if (message == NULL || strcmp(message, "\04") == 0){
        //         break;
        //     }
        //     write(client_fd, message, strlen(message));
        //     printf("Write : %s\n", message);
        //     free(message);

        //     char response[BUFFER_SIZE];
        //     int bytes_read = read(client_fd, response, BUFFER_SIZE - 1);
        //     if (bytes_read > 0) {
        //         response[bytes_read] = '\0';
        //         printf("Server response: %s\n", response);
        //     }
        // }


        client_stop(client_fd);
    } else {
        print_usage();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
