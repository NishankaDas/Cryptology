#include <stdio.h>
#include <string.h>

#define N 256

// Swap function
void swap(unsigned char *a, unsigned char *b) {
    unsigned char temp = *a;
    *a = *b;
    *b = temp;
}

// Key Scheduling Algorithm (KSA)
void KSA(unsigned char *key, int keylen, unsigned char *S) {
    int i, j = 0;

    for (i = 0; i < N; i++)
        S[i] = i;

    for (i = 0; i < N; i++) {
        j = (j + S[i] + key[i % keylen]) % N;
        swap(&S[i], &S[j]);
    }
}

// Pseudo-Random Generation Algorithm (PRGA)
void PRGA(unsigned char *S, unsigned char *plaintext, unsigned char *ciphertext, int len) {
    int i = 0, j = 0, k, t;

    for (k = 0; k < len; k++) {
        i = (i + 1) % N;
        j = (j + S[i]) % N;

        swap(&S[i], &S[j]);

        t = (S[i] + S[j]) % N;
        unsigned char keyStream = S[t];

        ciphertext[k] = plaintext[k] ^ keyStream;
    }
}

// RC4 function (same for encryption & decryption)
void RC4(unsigned char *key, int keylen, unsigned char *input, unsigned char *output, int len) {
    unsigned char S[N];

    KSA(key, keylen, S);
    PRGA(S, input, output, len);
}

int main() {
    unsigned char key[] = "secretkey";
    unsigned char plaintext[] = "Hello RC4 Cipher";

    int len = strlen((char *)plaintext);

    unsigned char ciphertext[256];
    unsigned char decrypted[256];

    // Encryption
    RC4(key, strlen((char *)key), plaintext, ciphertext, len);

    printf("Ciphertext (in hex): ");
    for (int i = 0; i < len; i++) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n");

    // Decryption (same RC4 function)
    RC4(key, strlen((char *)key), ciphertext, decrypted, len);
    decrypted[len] = '\0';

    printf("Decrypted text: %s\n", decrypted);

    return 0;
}