# RC4 implementation in Python

def KSA(key):
    S = list(range(256))
    j = 0
    key_length = len(key)

    for i in range(256):
        j = (j + S[i] + key[i % key_length]) % 256
        S[i], S[j] = S[j], S[i]

    return S


def PRGA(S, data_len):
    i = 0
    j = 0

    for _ in range(data_len):
        i = (i + 1) % 256
        j = (j + S[i]) % 256

        S[i], S[j] = S[j], S[i]

        K = S[(S[i] + S[j]) % 256]
        yield K


def rc4(key, data):
    key = [ord(c) for c in key]
    S = KSA(key)
    keystream = PRGA(S, len(data))

    return bytes([c ^ k for c, k in zip(data, keystream)])


# ------------------ MAIN ------------------

if __name__ == "__main__":
    key = "secretkey"
    plaintext = "Hello RC4 Cipher"

    # Convert plaintext to bytes
    plaintext_bytes = plaintext.encode()

    # Encryption
    ciphertext = rc4(key, plaintext_bytes)
    print("Ciphertext (hex):", ciphertext.hex())

    # Decryption (same function)
    decrypted = rc4(key, ciphertext)
    print("Decrypted text:", decrypted.decode())