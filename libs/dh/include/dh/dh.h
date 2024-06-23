#ifndef __DH_DH_H__
#define __DH_DH_H__
// #-------------------------------#
// |           includes            |
// #-------------------------------#
// c includes
#include <openssl/evp.h>

// my includes

// #-------------------------------#
// |            macros             |
// #-------------------------------#

#define CURVE_NAME "prime256v1"

// #-------------------------------#
// | global types declarations     |
// #-------------------------------#

// #-------------------------------#
// | global variables declarations |
// #-------------------------------#

// #-------------------------------#
// | global function declarations  |
// #-------------------------------#

/**
 * @brief Handle OpenSSL errors and abort the program.
 */
void handleErrors(void);

/**
 * @brief Generate an EC key pair using the specified curve name.
 *
 * @param curve_name The name of the curve to use for key generation.
 * @return A pointer to the generated EVP_PKEY structure.
 */
EVP_PKEY* generate_key(const char* curve_name) ;

/**
 * @brief Convert binary data to a hexadecimal string.
 *
 * @param data The binary data to convert.
 * @param len The length of the binary data.
 * @return A string containing the hexadecimal representation of the data.
 */
char* get_hex(const unsigned char* data, size_t len);

/**
 * @brief Get the public key in hexadecimal format from an EVP_PKEY structure.
 *
 * @param pkey The EVP_PKEY structure containing the public key.
 * @return A string containing the hexadecimal representation of the public key.
 */
char* get_public_key_hex(EVP_PKEY* pkey);

/**
 * @brief Get the private key in hexadecimal format from an EVP_PKEY structure.
 *
 * @param pkey The EVP_PKEY structure containing the private key.
 * @return A string containing the hexadecimal representation of the private key.
 */
char* get_private_key_hex(EVP_PKEY* pkey);

/**
 * @brief Create an EVP_PKEY structure from a peer's public key in hexadecimal format.
 *
 * @param curve_name The name of the curve used for the peer's key.
 * @param hex_peer_public_key The hexadecimal representation of the peer's public key.
 * @return A pointer to the created EVP_PKEY structure.
 */
EVP_PKEY* create_peer_key(const char* curve_name, const char* hex_peer_public_key);

/**
 * @brief Derive a shared secret from a private key and a peer's public key.
 *
 * @param pkey The EVP_PKEY structure containing the private key.
 * @param hex_peer_public_key The hexadecimal representation of the peer's public key.
 * @return A string containing the hexadecimal representation of the derived shared secret.
 */
char* derive_shared_secret(EVP_PKEY* pkey, const char* hex_peer_public_key);

/**
 * @brief Derive an AES key from the shared secret using HKDF.
 *
 * @param shared_secret The shared secret from which the AES key will be derived.
 * @param key_length The length of the AES key to derive (128, 192, or 256 bits).
 * @return A string containing the hexadecimal representation of the derived AES key.
 */
char* derive_aes_key(const unsigned char* shared_secret, size_t shared_secret_len, int key_length);

// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#

#endif
