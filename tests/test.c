#include <stdio.h>
#include "test_aes_key.h"
#include "test_aes_cipher.h"

#define RUN(x) do{\
 printf("[Running] " #x "\n"); \
 x;\
 printf("[Done]\n"); \
 }while(0)

int main(void){
    RUN(test_aes_key());
    RUN(test_aes_key_expansion());
    RUN(test_aes_cipher());
    RUN(test_aes_transform());
    RUN(test_aes_inv_cipher());
    RUN(test_aes_2_way_128());
    RUN(test_aes_2_way_192());
    RUN(test_aes_2_way_256());

    return 0;
}
