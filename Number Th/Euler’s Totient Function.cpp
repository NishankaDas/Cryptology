#include <stdio.h>

// Function to compute GCD
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int eulerTotient(int n) {
    int count = 0;

    for (int i = 1; i <= n; i++) {
        if (gcd(i, n) == 1)
            count++;
    }

    return count;
}

int main() {
    int n;

    printf("Enter a number: ");
    scanf("%d", &n);

    printf("Euler Totient f(%d) = %d\n", n, eulerTotient(n));

    return 0;
}
