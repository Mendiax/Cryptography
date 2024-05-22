# Cryptography


# Performance
base implementation performance:
File size: 16 000 000 bytes (16 MB)
- Debug
    - Encryption time: 12.915076 seconds
    - Decryption time: 26.840614 seconds

- O2
    - Encryption time: 1.433219 seconds
    - Decryption time: 2.653374 seconds

- O2 (AVX in decryption)
    - Encryption time: 1.446191 seconds
    - Decryption time: 1.028710 seconds

- O3
    - Encryption time: 0.193356 seconds
    - Decryption time: 0.328518 seconds

- O3 (AVX in decryption)
    - Encryption time: 0.193057 seconds
    - Decryption time: 0.649261 seconds

In O3 avx is slower. This happens because in galois_mul() function we pass 1 variable and 1  constant. This creates possibility for compiler to inline code and create code for given constant. With AVX it is not possible so it is a bit slower.