
// #------------------------------#
// |          includes            |
// #------------------------------#
// c includes

// my includes
#include "dh/dh.h"

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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/param_build.h>
#include <openssl/core_names.h>
#include <openssl/err.h>
#include <openssl/kdf.h>


void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

EVP_PKEY* generate_key(const char* curve_name) {
    EVP_PKEY_CTX* ctx;
    EVP_PKEY* pkey = NULL;
    OSSL_PARAM_BLD* param_bld;
    OSSL_PARAM* params;

    ctx = EVP_PKEY_CTX_new_from_name(NULL, "EC", NULL);
    if (!ctx)
        handleErrors();

    if (EVP_PKEY_keygen_init(ctx) <= 0)
        handleErrors();

    param_bld = OSSL_PARAM_BLD_new();
    if (!param_bld)
        handleErrors();

    if (!OSSL_PARAM_BLD_push_utf8_string(param_bld, OSSL_PKEY_PARAM_GROUP_NAME, curve_name, 0))
        handleErrors();

    params = OSSL_PARAM_BLD_to_param(param_bld);
    if (!params)
        handleErrors();

    if (!EVP_PKEY_CTX_set_params(ctx, params))
        handleErrors();

    if (EVP_PKEY_generate(ctx, &pkey) <= 0)
        handleErrors();

    OSSL_PARAM_BLD_free(param_bld);
    OSSL_PARAM_free(params);
    EVP_PKEY_CTX_free(ctx);

    return pkey;
}

char* get_hex(const unsigned char* data, size_t len) {
    char* hex = malloc(len * 2 + 1);
    for (size_t i = 0; i < len; i++) {
        sprintf(hex + i * 2, "%02x", data[i]);
    }
    return hex;
}

char* get_public_key_hex(EVP_PKEY* pkey) {
    size_t len = 0;
    unsigned char* pubkey = NULL;

    if (!EVP_PKEY_get_octet_string_param(pkey, OSSL_PKEY_PARAM_PUB_KEY, NULL, 0, &len))
        handleErrors();

    pubkey = malloc(len);
    if (!pubkey)
        handleErrors();

    if (!EVP_PKEY_get_octet_string_param(pkey, OSSL_PKEY_PARAM_PUB_KEY, pubkey, len, &len))
        handleErrors();

    char* hex = get_hex(pubkey, len);
    free(pubkey);
    return hex;
}


char* get_private_key_hex(EVP_PKEY* pkey) {
    BIGNUM* privkey = NULL;
    char* hex = NULL;

    if (!EVP_PKEY_get_bn_param(pkey, OSSL_PKEY_PARAM_PRIV_KEY, &privkey))
        handleErrors();

    hex = BN_bn2hex(privkey);
    BN_free(privkey);
    return hex;
}


EVP_PKEY* create_peer_key(const char* curve_name, const char* hex_peer_public_key) {
    EVP_PKEY_CTX* peer_key_ctx;
    EVP_PKEY* peer_key = NULL;
    OSSL_PARAM_BLD* param_bld;
    OSSL_PARAM* params;
    size_t len = strlen(hex_peer_public_key) / 2;
    unsigned char* pubkey = malloc(len);

    for (size_t i = 0; i < len; i++) {
        sscanf(hex_peer_public_key + 2 * i, "%2hhx", &pubkey[i]);
    }

    param_bld = OSSL_PARAM_BLD_new();
    if (!param_bld)
        handleErrors();

    if (!OSSL_PARAM_BLD_push_utf8_string(param_bld, OSSL_PKEY_PARAM_GROUP_NAME, curve_name, 0))
        handleErrors();

    if (!OSSL_PARAM_BLD_push_octet_string(param_bld, OSSL_PKEY_PARAM_PUB_KEY, pubkey, len))
        handleErrors();

    params = OSSL_PARAM_BLD_to_param(param_bld);
    if (!params)
        handleErrors();

    peer_key_ctx = EVP_PKEY_CTX_new_from_name(NULL, "EC", NULL);
    if (!peer_key_ctx)
        handleErrors();

    if (EVP_PKEY_fromdata_init(peer_key_ctx) <= 0)
        handleErrors();

    if (EVP_PKEY_fromdata(peer_key_ctx, &peer_key, EVP_PKEY_PUBLIC_KEY, params) <= 0)
        handleErrors();

    OSSL_PARAM_BLD_free(param_bld);
    OSSL_PARAM_free(params);
    EVP_PKEY_CTX_free(peer_key_ctx);
    free(pubkey);

    return peer_key;
}

char* derive_shared_secret(EVP_PKEY* pkey, const char* hex_peer_public_key) {
    EVP_PKEY_CTX* derivation_ctx;
    EVP_PKEY* peer_key = create_peer_key(CURVE_NAME, hex_peer_public_key);
    unsigned char* shared_secret = NULL;
    size_t shared_secret_len = 0;
    char* hex_shared_secret;

    derivation_ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if (!derivation_ctx)
        handleErrors();

    if (EVP_PKEY_derive_init(derivation_ctx) <= 0)
        handleErrors();

    if (EVP_PKEY_derive_set_peer(derivation_ctx, peer_key) <= 0)
        handleErrors();

    if (EVP_PKEY_derive(derivation_ctx, NULL, &shared_secret_len) <= 0)
        handleErrors();

    shared_secret = malloc(shared_secret_len);
    if (!shared_secret)
        handleErrors();

    if (EVP_PKEY_derive(derivation_ctx, shared_secret, &shared_secret_len) <= 0)
        handleErrors();

    hex_shared_secret = get_hex(shared_secret, shared_secret_len);
    free(shared_secret);
    EVP_PKEY_free(peer_key);
    EVP_PKEY_CTX_free(derivation_ctx);

    return hex_shared_secret;
}

unsigned char* derive_aes_key(const unsigned char* shared_secret, size_t shared_secret_len, int key_length) {
    EVP_KDF* kdf;
    EVP_KDF_CTX* kctx;
    unsigned char* aes_key;
    const size_t aes_key_len = key_length / 8;

    kdf = EVP_KDF_fetch(NULL, "HKDF", NULL);
    if (!kdf)
        handleErrors();

    kctx = EVP_KDF_CTX_new(kdf);
    if (!kctx)
        handleErrors();

    OSSL_PARAM params[] = {
        OSSL_PARAM_construct_utf8_string(OSSL_KDF_PARAM_DIGEST, SN_sha256, 0),
        OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_KEY, (void*)shared_secret, shared_secret_len),
        OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_SALT, "salt", 4),
        OSSL_PARAM_construct_end()
    };

    if (EVP_KDF_CTX_set_params(kctx, params) <= 0)
        handleErrors();

    aes_key = malloc(aes_key_len);
    if (!aes_key)
        handleErrors();

    if (EVP_KDF_derive(kctx, aes_key, aes_key_len, NULL) <= 0)
        handleErrors();
  
    EVP_KDF_free(kdf);
    EVP_KDF_CTX_free(kctx);

    return aes_key;
}

unsigned char* convert_hex_str_to_bytes(const char* hex_str) {
    const size_t shared_secret_len = strlen(hex_str) / 2;
    unsigned char* shared_secret_bin = malloc(shared_secret_len);
    for (size_t i = 0; i < shared_secret_len; i++) {
        sscanf(hex_str + 2 * i, "%2hhx", &shared_secret_bin[i]);
    }
    return  shared_secret_bin;
}

// #------------------------------#
// | static functions definitions |
// #------------------------------#
