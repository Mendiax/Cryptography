#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "server/msg.h"

int main()
{

    {
        const char* payload_original = "payload_original";
        PlainMsg* test_msg = new_plain_msg(payload_original, strlen(payload_original) + 1);

        PlainMsg* test_msg_serialized = plain_msg_serialize(test_msg);
        // send test_msg_serialized->payload_size bytes

        PlainMsg* deserialized = plain_msg_deserialize(test_msg_serialized->payload);
        assert(memcmp(&deserialized->payload_size, &test_msg->payload_size, sizeof(test_msg->payload_size)) == 0);
        assert(strcmp(deserialized->payload, test_msg->payload) == 0);

        printf("%s\n", deserialized->payload);
        printf("%s\n", test_msg->payload);


        delete_plain_msg(&test_msg);
        delete_plain_msg(&deserialized);
        delete_plain_msg(&test_msg_serialized);

    }
    {
        const char* payload_original = "payload_original";
        ServerMsg* test_msg = new_server_msg(1, MESSAGE_DESTINATION_INIT, payload_original, strlen(payload_original) + 1);

        PlainMsg* test_msg_serialized = server_msg_serialize(test_msg);
        // send test_msg_serialized->payload_size bytes

        ServerMsg* deserialized = server_msg_deserialize(test_msg_serialized->payload);
        assert(memcmp(&deserialized->destination, &test_msg->destination, sizeof(deserialized->destination)) == 0);
        assert(memcmp(&deserialized->token, &test_msg->token, sizeof(deserialized->token)) == 0);
        assert(memcmp(&deserialized->msg_p->payload_size, &test_msg->msg_p->payload_size, sizeof(deserialized->msg_p->payload_size)) == 0);
        assert(strcmp(deserialized->msg_p->payload,      test_msg->msg_p->payload) == 0);
        printf("%s\n", deserialized->msg_p->payload);
        printf("%s\n", test_msg->msg_p->payload);



        delete_server_msg(&test_msg);
        delete_plain_msg(&test_msg_serialized);
        delete_server_msg(&deserialized);
    }
}