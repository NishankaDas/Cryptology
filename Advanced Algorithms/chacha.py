# ChaCha20 implementation in Python

def rotl(v, c):
    return ((v << c) & 0xffffffff) | (v >> (32 - c))


def quarter_round(a, b, c, d):
    a = (a + b) & 0xffffffff; d ^= a; d = rotl(d, 16)
    c = (c + d) & 0xffffffff; b ^= c; b = rotl(b, 12)
    a = (a + b) & 0xffffffff; d ^= a; d = rotl(d, 8)
    c = (c + d) & 0xffffffff; b ^= c; b = rotl(b, 7)
    return a, b, c, d


def chacha20_block(state):
    x = state.copy()

    for _ in range(10):  # 20 rounds (2 per loop)
        # Column rounds
        x[0], x[4], x[8], x[12] = quarter_round(x[0], x[4], x[8], x[12])
        x[1], x[5], x[9], x[13] = quarter_round(x[1], x[5], x[9], x[13])
        x[2], x[6], x[10], x[14] = quarter_round(x[2], x[6], x[10], x[14])
        x[3], x[7], x[11], x[15] = quarter_round(x[3], x[7], x[11], x[15])

        # Diagonal rounds
        x[0], x[5], x[10], x[15] = quarter_round(x[0], x[5], x[10], x[15])
        x[1], x[6], x[11], x[12] = quarter_round(x[1], x[6], x[11], x[12])
        x[2], x[7], x[8], x[13] = quarter_round(x[2], x[7], x[8], x[13])
        x[3], x[4], x[9], x[14] = quarter_round(x[3], x[4], x[9], x[14])

    return [(x[i] + state[i]) & 0xffffffff for i in range(16)]


def serialize(block):
    return b''.join(x.to_bytes(4, 'little') for x in block)


def chacha20_encrypt(key, nonce, counter, plaintext):
    # Constants
    constants = [0x61707865, 0x3320646e, 0x79622d32, 0x6b206574]

    # Convert key and nonce
    key_words = [int.from_bytes(key[i:i+4], 'little') for i in range(0, 32, 4)]
    nonce_words = [int.from_bytes(nonce[i:i+4], 'little') for i in range(0, 12, 4)]

    ciphertext = b''
    block_count = 0

    for i in range(0, len(plaintext), 64):
        state = constants + key_words + [counter + block_count] + nonce_words

        block = chacha20_block(state)
        keystream = serialize(block)

        chunk = plaintext[i:i+64]
        encrypted_chunk = bytes([c ^ k for c, k in zip(chunk, keystream)])

        ciphertext += encrypted_chunk
        block_count += 1

    return ciphertext


# ------------------ MAIN ------------------

if __name__ == "__main__":
    key = b"thisis32bytekeyforchacha20!!!!"  # 32 bytes
    nonce = b"123456789012"  # 12 bytes
    counter = 1

    plaintext = b"It's Me Nishanka!"

    # Encrypt
    ciphertext = chacha20_encrypt(key, nonce, counter, plaintext)
    print("Ciphertext (hex):", ciphertext.hex())

    # Decrypt (same function)
    decrypted = chacha20_encrypt(key, nonce, counter, ciphertext)
    print("Decrypted:", decrypted.decode())