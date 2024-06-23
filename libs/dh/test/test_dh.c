#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/dh.h>
#include <openssl/engine.h>
#include <openssl/err.h>

typedef struct {
    DH *dh;
    BIGNUM *pub_key;
} DH_Party;

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

void print_bytes(const char *label, const unsigned char *bytes, size_t len) {
    printf("%s: ", label);
    for (size_t i = 0; i < len; ++i) {
        printf("%02x", bytes[i]);
    }
    printf("\n");
}

void initialize_dh_party(DH_Party *party, int is_server, DH *server_dh) {
    if (is_server) {
        party->dh = DH_new();
        if (!party->dh) handleErrors();

        // Generate parameters
        if (DH_generate_parameters_ex(party->dh, 2048, DH_GENERATOR_2, NULL) != 1)
            handleErrors();
    } else {
        party->dh = DHparams_dup(server_dh);
        if (!party->dh) handleErrors();
    }

    // Generate key pair
    if (DH_generate_key(party->dh) != 1) handleErrors();

    // Get public key
    party->pub_key = BN_dup(DH_get0_pub_key(party->dh));
    if (!party->pub_key) handleErrors();
}

unsigned char* compute_shared_secret(DH_Party *party, const BIGNUM *peer_pub_key, int *secret_len) {
    unsigned char *shared_secret = malloc(DH_size(party->dh));
    if (!shared_secret) handleErrors();

    *secret_len = DH_compute_key(shared_secret, peer_pub_key, party->dh);
    if (*secret_len == -1) handleErrors();

    return shared_secret;
}

int main() {
    DH_Party server, client;
    int server_secret_len, client_secret_len;

    // Initialize server and client
    initialize_dh_party(&server, 1, NULL);
    initialize_dh_party(&client, 0, server.dh);

    // Compute shared secrets
    unsigned char *server_shared_secret = compute_shared_secret(&server, client.pub_key, &server_secret_len);
    unsigned char *client_shared_secret = compute_shared_secret(&client, server.pub_key, &client_secret_len);

    // Print shared secrets
    print_bytes("Server shared secret", server_shared_secret, server_secret_len);
    print_bytes("Client shared secret", client_shared_secret, client_secret_len);

    // Cleanup
    DH_free(server.dh);
    BN_free(server.pub_key);
    free(server_shared_secret);

    DH_free(client.dh);
    BN_free(client.pub_key);
    free(client_shared_secret);

    return 0;
}
