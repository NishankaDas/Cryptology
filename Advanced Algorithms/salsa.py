# Salsa20 implementation in Python

def rotl(v, c):
    return ((v << c) & 0xffffffff) | (v >> (32 - c))


def quarter_round(y0, y1, y2, y3):
    y1 ^= rotl((y0 + y3) & 0xffffffff, 7)
    y2 ^= rotl((y1 + y0) & 0xffffffff, 9)
    y3 ^= rotl((y2 + y1) & 0xffffffff, 13)
    y0 ^= rotl((y3 + y2) & 0xffffffff, 18)
    return y0, y1, y2, y3


def row_round(y):
    y[0], y[1], y[2], y[3] = quarter_round(y[0], y[1], y[2], y[3])
    y[5], y[6], y[7], y[4] = quarter_round(y[5], y[6], y[7], y[4])
    y[10], y[11], y[8], y[9] = quarter_round(y[10], y[11], y[8], y[9])
    y[15], y[12], y[13], y[14] = quarter_round(y[15], y[12], y[13], y[14])
    return y


def column_round(x):
    x[0], x[4], x[8], x[12] = quarter_round(x[0], x[4], x[8], x[12])
    x[5], x[9], x[13], x[1] = quarter_round(x[5], x[9], x[13], x[1])
    x[10], x[14], x[2], x[6] = quarter_round(x[10], x[14], x[2], x[6])
    x[15], x[3], x[7], x[11] = quarter_round(x[15], x[3], x[7], x[11])
    return x


def double_round(x):
    x = column_round(x)
    x = row_round(x)
    return x


def salsa20_block(state):
    x = state.copy()

    for _ in range(10):  # 20 rounds
        x = double_round(x)

    return [(x[i] + state[i]) & 0xffffffff for i in range(16)]


def serialize(block):
    return b''.join(x.to_bytes(4, 'little') for x in block)


def salsa20_encrypt(key, nonce, counter, plaintext):
    # Constants ("expand 32-byte k")
    constants = [
        0x61707865, 0x3320646e, 0x79622d32, 0x6b206574
    ]

    key_words = [int.from_bytes(key[i:i+4], 'little') for i in range(0, 32, 4)]
    nonce_words = [int.from_bytes(nonce[i:i+4], 'little') for i in range(0, 8, 4)]

    ciphertext = b''
    block_count = 0

    for i in range(0, len(plaintext), 64):
        state = [
            constants[0], key_words[0], key_words[1], key_words[2],
            key_words[3], constants[1], nonce_words[0], nonce_words[1],
            counter + block_count, 0,
            constants[2], key_words[4], key_words[5], key_words[6],
            key_words[7], constants[3]
        ]

        block = salsa20_block(state)
        keystream = serialize(block)

        chunk = plaintext[i:i+64]
        encrypted_chunk = bytes([c ^ k for c, k in zip(chunk, keystream)])

        ciphertext += encrypted_chunk
        block_count += 1

    return ciphertext


# ------------------ MAIN ------------------

if __name__ == "__main__":
    key = b"thisis32bytekeyforsalsa20cipher!!"  # 32 bytes
    nonce = b"12345678"  # 8 bytes
    counter = 1

    plaintext = b"Sasa Nachte Jani na"

    # Encrypt
    ciphertext = salsa20_encrypt(key, nonce, counter, plaintext)
    print("Ciphertext (hex):", ciphertext.hex())

    # Decrypt (same function)
    decrypted = salsa20_encrypt(key, nonce, counter, ciphertext)
    print("Decrypted:", decrypted.decode())