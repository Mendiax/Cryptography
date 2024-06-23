#include "dh/dh.h"
#include <string.h>

int main() {
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    EVP_PKEY* server_key = generate_key(CURVE_NAME);
    EVP_PKEY* client_key = generate_key(CURVE_NAME);

    char* server_private_key = get_private_key_hex(server_key);
    char* server_public_key = get_public_key_hex(server_key);
    char* client_private_key = get_private_key_hex(client_key);
    char* client_public_key = get_public_key_hex(client_key);

    printf("Server Private Key: %s\n", server_private_key);
    printf("Server Public Key: %s\n", server_public_key);
    printf("Client Private Key: %s\n", client_private_key);
    printf("Client Public Key: %s\n", client_public_key);

    char* server_shared_secret = derive_shared_secret(server_key, client_public_key);
    char* client_shared_secret = derive_shared_secret(client_key, server_public_key);

    printf("Server Shared Secret: %s\n", server_shared_secret);
    printf("Client Shared Secret: %s\n", client_shared_secret);

    if (strcmp(server_shared_secret, client_shared_secret) == 0) {
        printf("Shared secrets match.\n");
    } else {
        printf("Shared secrets do not match.\n");
    }

    size_t shared_secret_len = strlen(server_shared_secret) / 2;
    unsigned char* shared_secret_bin = malloc(shared_secret_len);
    for (size_t i = 0; i < shared_secret_len; i++) {
        sscanf(server_shared_secret + 2 * i, "%2hhx", &shared_secret_bin[i]);
    }

    char* aes_key_128 = derive_aes_key(shared_secret_bin, shared_secret_len, 128);
    char* aes_key_192 = derive_aes_key(shared_secret_bin, shared_secret_len, 192);
    char* aes_key_256 = derive_aes_key(shared_secret_bin, shared_secret_len, 256);

    printf("AES-128 Key: %s\n", aes_key_128);
    printf("AES-192 Key: %s\n", aes_key_192);
    printf("AES-256 Key: %s\n", aes_key_256);

    EVP_PKEY_free(server_key);
    EVP_PKEY_free(client_key);
    OPENSSL_free(server_private_key);
    OPENSSL_free(server_public_key);
    OPENSSL_free(client_private_key);
    OPENSSL_free(client_public_key);
    free(server_shared_secret);
    free(client_shared_secret);
    free(shared_secret_bin);
    free(aes_key_128);
    free(aes_key_192);
    free(aes_key_256);

    EVP_cleanup();
    ERR_free_strings();

    return 0;
}
