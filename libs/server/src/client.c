
// #------------------------------#
// |          includes            |
// #------------------------------#
// c includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// my includes
#include "server/client.h"
#include "server/msg.h"
#include "server/utils.h"



// #------------------------------#
// |           macros             |
// #------------------------------#
#define BUFFER_SIZE 1000000

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

PlainMsg* client_send_msg(int client_fd, ServerMsg* msg_p) {

    // send msg
    PlainMsg* msg_to_server =  server_msg_serialize(msg_p);
    write_with_size_prefix(client_fd, msg_to_server->payload, msg_to_server->payload_size);
    delete_plain_msg(&msg_to_server);

    char* buffer;
    uint16_t size;
    if(read_with_size_prefix(client_fd, &buffer, &size)) {
        perror("Error reading from socket");
    }
    PlainMsg* response = new_plain_msg(buffer, size);
    free(buffer);
    return response;
}

int client_start(const char *server_ip, int port) {
    int client_fd;
    struct sockaddr_in server_addr;

    // Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("invalid address or address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    return client_fd;
}

void client_stop(int client_fd) {
    close(client_fd);
}


// #------------------------------#
// | static functions definitions |
// #------------------------------#
