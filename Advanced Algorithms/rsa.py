import random
from math import gcd

# ------------------ HELPER FUNCTIONS ------------------

# Modular inverse using Extended Euclidean Algorithm
def mod_inverse(e, phi):
    def extended_gcd(a, b):
        if b == 0:
            return a, 1, 0
        g, x1, y1 = extended_gcd(b, a % b)
        x = y1
        y = x1 - (a // b) * y1
        return g, x, y

    g, x, _ = extended_gcd(e, phi)
    if g != 1:
        raise Exception("Modular inverse does not exist")
    return x % phi


# Check for prime (simple version for learning)
def is_prime(n):
    if n < 2:
        return False
    for i in range(2, int(n**0.5) + 1):
        if n % i == 0:
            return False
    return True


# Generate a random prime number (small for demo)
def generate_prime(start=100, end=300):
    while True:
        num = random.randint(start, end)
        if is_prime(num):
            return num


# ------------------ RSA KEY GENERATION ------------------

def generate_keys():
    p = generate_prime()
    q = generate_prime()

    while q == p:
        q = generate_prime()

    n = p * q
    phi = (p - 1) * (q - 1)

    # Choose e such that gcd(e, phi) = 1
    e = random.randrange(2, phi)
    while gcd(e, phi) != 1:
        e = random.randrange(2, phi)

    # Compute d
    d = mod_inverse(e, phi)

    return (e, n), (d, n)


# ------------------ ENCRYPTION ------------------

def encrypt(plaintext, public_key):
    e, n = public_key
    return [pow(ord(char), e, n) for char in plaintext]


# ------------------ DECRYPTION ------------------

def decrypt(ciphertext, private_key):
    d, n = private_key
    return ''.join([chr(pow(char, d, n)) for char in ciphertext])


# ------------------ MAIN ------------------

if __name__ == "__main__":
    public_key, private_key = generate_keys()

    print("Public Key (e, n):", public_key)
    print("Private Key (d, n):", private_key)

    message = "HELLO RSA"

    # Encrypt
    encrypted_msg = encrypt(message, public_key)
    print("Encrypted:", encrypted_msg)

    # Decrypt
    decrypted_msg = decrypt(encrypted_msg, private_key)
    print("Decrypted:", decrypted_msg)