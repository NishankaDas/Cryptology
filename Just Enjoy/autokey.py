
import numpy as np

def enc(key, text):
    text = text.upper()
    key = int(key)

    main = []
    for ch in text:
        main.append(ord(ch) - ord('A'))

    y = [key]
    for i in range(len(text) - 1):
        y.append(main[i])

    enc_f = []
    for i in range(len(text)):
        enc_f.append((main[i] + y[i]) % 26)

    return ''.join(chr(x + ord('A')) for x in enc_f)


def denc(key, text):
    text = text.upper()
    key = int(key)

    org = []
    for i in range(len(text)):
        p = (ord(text[i]) - ord('A') - key) % 26
        org.append(chr(p + ord('A')))

        key = p

    return ''.join(org)




text = input("Enter a string: ")
key = input("Enter key: ")

cipher = enc(key, text)
print("Ciphertext:", cipher)

normal = denc(key,cipher)
print("Decrypted : ", normal)
