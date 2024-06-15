#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "server/client.h"
#include "server/server.h"

void print_usage() {
    printf("Usage:\n");
    printf("  To run as server: ./main server <port>\n");
    printf("  To run as client: ./main client <server_ip> <port>\n");
}

// Function to handle client requests
void *handle_client(void *arg) {
    int client_socket = *((int *)arg);
    char buffer[BUFFER_SIZE] = {0};
    int valread;

    while (1) {
        // Read client request
        valread = read(client_socket, buffer, BUFFER_SIZE);
        if (valread < 0) {
            perror("Error reading from socket");
            close(client_socket);
            pthread_exit(NULL);
        }

        // Process client request (for simplicity, echo back the same data)
        printf("Client message: %s\n", buffer);

        // Echo back to client
        if (write(client_socket, buffer, strlen(buffer)) < 0) {
            perror("Error writing to socket");
        }
    }

    // Close client socket
    close(client_socket);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage();
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "server") == 0) {
        // Run as server
        int port = atoi(argv[2]);
        server_start(port, handle_client);
    } else if (strcmp(argv[1], "client") == 0) {
        // Run as client
        const char *server_ip = argv[2];
        int port = atoi(argv[3]);
        int client_fd = client_start(server_ip, port);

        // Example client interaction: send a message and receive response
        char* message = NULL;
        while (1) { // ^D
            scanf("%ms", &message);
            if (message == NULL || strcmp(message, "\04") == 0){
                break;
            }
            write(client_fd, message, strlen(message));
            printf("Write : %s\n", message);
            free(message);

            char response[BUFFER_SIZE];
            int bytes_read = read(client_fd, response, BUFFER_SIZE - 1);
            if (bytes_read > 0) {
                response[bytes_read] = '\0';
                printf("Server response: %s\n", response);
            }
        }


        client_stop(client_fd);
    } else {
        print_usage();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
