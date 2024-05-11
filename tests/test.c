#include "test_aes_key.h"
#include "test_aes_cipher.h"

int main(void){

    test_aes_key();
    test_aes_key_expansion();
    test_aes_cipher();
    return 0;
}
