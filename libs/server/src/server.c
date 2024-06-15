
// #------------------------------#
// |          includes            |
// #------------------------------#
// c includes
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

// my includes
#include "server/server.h"

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

void server_start(int port, ServerRequestHandler request_handler) {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(server_addr);
    pthread_t thread_id;
    int opt = 1;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);


    // Bind socket to address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", port);

    // Accept incoming connections
    while (1) {
        printf("wait for connection\n");
        if ((client_fd = accept(server_fd, (struct sockaddr *)&server_addr, (socklen_t*)&addr_len)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }
        printf("accepted connection\n");

        // Create a new thread to handle the client
        if (pthread_create(&thread_id, NULL, request_handler, (void *)&client_fd) != 0) {
            perror("Failed to create thread");
            close(client_fd);
        }
    }

    // Close server socket (not reachable in this example)
    close(server_fd);
}


// #------------------------------#
// | static functions definitions |
// #------------------------------#
