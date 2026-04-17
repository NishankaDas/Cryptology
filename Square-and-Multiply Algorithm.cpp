#include <stdio.h>


int decimalToBinary(int decimal, int binary[]) {
    int i = 0;

    if (decimal == 0) {
        binary[i++] = 0;
        return i;
    }

    while (decimal > 0) {
        binary[i++] = decimal % 2;
        decimal /= 2;
    }

    // reverse
    for (int j = 0; j < i / 2; j++) {
        int temp = binary[j];
        binary[j] = binary[i - j - 1];
        binary[i - j - 1] = temp;
    }

    return i;
}

int mod(int a, int b, int c) {
    int z[32], q[32];
    int final = 1;

    int size = decimalToBinary(b, z);

    q[0] = a % c;
    
	// finding the mods and storing in array
    for (int i = 1; i < size; i++) {
        q[i] = (q[i - 1] * q[i - 1]) % c;
    }
	//finding the final mod
    for (int i = 0; i < size; i++) {
        if (z[size - i - 1] == 1) {
            final = (final * q[i]) % c;
        }
    }

    return final;
}

int main() {
    int x, y, z;

    printf("Enter three values : ");
    scanf("%d%d%d", &x, &y, &z);

    printf("The answer in O(LogN) is : %d\n", mod(x, y, z));
    return 0;
}

