#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to compute gcd
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Modular exponentiation (square and multiply)
long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod;

    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;

        base = (base * base) % mod;
        exp = exp / 2;
    }
    return result;
}

// Extended Euclidean Algorithm for modular inverse
long long mod_inverse(long long e, long long phi) {
    long long t = 0, newt = 1;
    long long r = phi, newr = e;

    while (newr != 0) {
        long long q = r / newr;

        long long temp = newt;
        newt = t - q * newt;
        t = temp;

        temp = newr;
        newr = r - q * newr;
        r = temp;
    }

    if (r > 1) {
        printf("Inverse does not exist\n");
        exit(1);
    }

    if (t < 0)
        t += phi;

    return t;
}

// Simple prime check
int is_prime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

// Generate small random prime (for demo)
int generate_prime() {
    while (1) {
        int num = rand() % 100 + 50; // range 50–149
        if (is_prime(num))
            return num;
    }
}

int main() {
    srand(time(NULL));

    int p = generate_prime();
    int q = generate_prime();

    while (q == p)
        q = generate_prime();

    long long n = (long long)p * q;
    long long phi = (long long)(p - 1) * (q - 1);

    // Choose e
    long long e = 2;
    while (e < phi) {
        if (gcd(e, phi) == 1)
            break;
        e++;
    }

    // Compute d
    long long d = mod_inverse(e, phi);

    printf("Public Key (e, n): (%lld, %lld)\n", e, n);
    printf("Private Key (d, n): (%lld, %lld)\n", d, n);

    char message[] = "HELLO";
    int len = sizeof(message) - 1;

    long long encrypted[100];

    // Encryption
    printf("Encrypted: ");
    for (int i = 0; i < len; i++) {
        encrypted[i] = mod_exp(message[i], e, n);
        printf("%lld ", encrypted[i]);
    }
    printf("\n");

    // Decryption
    printf("Decrypted: ");
    for (int i = 0; i < len; i++) {
        char decrypted_char = (char)mod_exp(encrypted[i], d, n);
        printf("%c", decrypted_char);
    }
    printf("\n");

    return 0;
}