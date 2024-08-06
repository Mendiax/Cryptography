
// #------------------------------#
// |          includes            |
// #------------------------------#
// c includes
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <assert.h>

// my includes
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

// #------------------------------#
// | static functions declarations|
// #------------------------------#

// #------------------------------#
// | global function definitions  |
// #------------------------------#

// #------------------------------#
// | static functions definitions |
// #------------------------------#

PlainMsg *new_plain_msg(const char *payload, size_t payload_size) {
    PlainMsg* new_plain_msg_p = malloc(sizeof(*new_plain_msg_p) + payload_size);
    memcpy(new_plain_msg_p->payload, payload, payload_size);
    new_plain_msg_p->payload_size = payload_size;
    return new_plain_msg_p;
}

PlainMsg *new_plain_msg_empty(size_t payload_size) {
    PlainMsg* new_plain_msg_p = malloc(sizeof(*new_plain_msg_p) + payload_size);
    new_plain_msg_p->payload_size = payload_size;
    return new_plain_msg_p;
}

void delete_plain_msg(PlainMsg** plain_msg_pp) {
    if (plain_msg_pp == NULL || *plain_msg_pp == NULL) {return;}
    free(*plain_msg_pp);
    *plain_msg_pp = NULL;
}
size_t plain_msg_get_size(const PlainMsg * plain_msg_p) {
    return (size_t)sizeof(PlainMsg) + (size_t)plain_msg_p->payload_size;
}




ServerMsg* new_server_msg(SessionToken session, MessageDestination destination, const char* payload, size_t payload_size)
{
    ServerMsg* new_msg_p = malloc(sizeof(*new_msg_p));
    new_msg_p->msg_p = new_plain_msg(payload, payload_size);
    new_msg_p->destination = destination;
    new_msg_p->token = session;
    return new_msg_p;
}

void delete_server_msg(ServerMsg** msg_pp) {
    delete_plain_msg(&(*msg_pp)->msg_p);
    if (msg_pp == NULL || *msg_pp == NULL) {return;}
    free(*msg_pp);
    *msg_pp = NULL;
}
size_t server_msg_get_size(const ServerMsg * server_msg_p){
    return sizeof(ServerMsg) + plain_msg_get_size(server_msg_p->msg_p);
}



ClientMsg *new_client_msg(const void *payload, size_t payload_size) {
    ClientMsg* new_msg_p = malloc(sizeof(*new_msg_p));
    new_msg_p->msg_p = new_plain_msg(payload, payload_size);
    return new_msg_p;
}

void delete_client_msg(ClientMsg** msg_pp) {
    delete_plain_msg(&(*msg_pp)->msg_p);
    if (msg_pp == NULL || *msg_pp == NULL) {return;}
    free(*msg_pp);
    *msg_pp = NULL;
}

// ------------ Serialization ------------------------
// Payload size is uint16_t
_Static_assert(sizeof((PlainMsg){0}.payload_size) == sizeof(uint16_t), "Change htons()!");

PlainMsg* plain_msg_serialize(const PlainMsg *msg) {
    const size_t msg_size = plain_msg_get_size(msg);
    PlainMsg *buffer = new_plain_msg((char*)msg, msg_size);
    if (buffer == NULL) {
        return NULL;
    }
    // swap byte order
    const uint16_t payload_size = htons(msg->payload_size);
    memcpy(buffer->payload, &payload_size, sizeof(payload_size));

    return buffer;
}

PlainMsg* plain_msg_deserialize(const char *buffer) {
    const uint16_t payload_size = ntohs(*(uint16_t *)buffer);
    PlainMsg *msg = new_plain_msg_empty(payload_size);
    if (msg == NULL) {
        return NULL;
    }
    memcpy(msg->payload, buffer + sizeof(uint16_t), payload_size);
    return msg;
}

PlainMsg* server_msg_serialize(const ServerMsg *msg_p) {
    const size_t plain_msg_size = server_msg_get_size(msg_p);

    // create space
    PlainMsg *buffer = new_plain_msg_empty(plain_msg_size);
    if (buffer == NULL) {
        return NULL;
    }
    _Static_assert(sizeof(msg_p->destination) == sizeof(uint32_t),
                   "Change htonl()!");
    _Static_assert(sizeof(msg_p->token) == sizeof(uint64_t), "Change htonll()!");

    const uint32_t destination_be = htonl(msg_p->destination);
    const uint64_t token_be = htonll(msg_p->token);

    memcpy(buffer->payload, &destination_be, sizeof(destination_be));
    memcpy(buffer->payload + sizeof(destination_be), &token_be,
           sizeof(token_be));

    PlainMsg* serialized_payload = plain_msg_serialize(msg_p->msg_p);

    assert(sizeof(ServerMsg) + serialized_payload->payload_size == plain_msg_size);
    char* plain_msg_space = buffer->payload + sizeof(ServerMsg);
    memcpy(plain_msg_space, serialized_payload->payload, serialized_payload->payload_size);
    delete_plain_msg(&serialized_payload);

    return buffer;
}

ServerMsg* server_msg_deserialize(const char *buffer) {
    ServerMsg *msg = malloc(sizeof(ServerMsg));
    if (msg == NULL) {
        return NULL;
    }
    msg->destination = ntohl(*(uint32_t *)buffer);
    msg->token = ntohll(*(uint64_t *)(buffer + sizeof(msg->destination)));
    msg->msg_p = plain_msg_deserialize(buffer + sizeof(ServerMsg));
    return msg;
}

PlainMsg* client_msg_serialize(const ClientMsg *msg) {
    return plain_msg_serialize(msg->msg_p);
}

ClientMsg* client_msg_deserialize(const char *buffer) {
    ClientMsg *msg = malloc(sizeof(ClientMsg));
    if (msg == NULL) {
        return NULL;
    }
    msg->msg_p = plain_msg_deserialize(buffer);
    return msg;
}