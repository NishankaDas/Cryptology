import numpy
import math

import math
import numpy as np

def encryp(key, text):
    K = np.array(key)
    n = len(K)
    
    # 1. Check determinant != 0 and 2. Coprime with 26
    det = int(round(np.linalg.det(K))) % 26
    if det == 0 or math.gcd(det, 26) != 1:
        raise ValueError("Invalid key: determinant has no inverse mod 26.")

    # 3. Text to numbers 0-25 & pad with 'X'
    text = text.upper().replace(" ", "")
    if len(text) % n != 0:
        text += "X" * (n - len(text) % n)
    nums = [ord(c) - ord('A') for c in text]

    # 4. Divide into parts, matrix multiply, and collect
    res = []
    for i in range(0, len(nums), n):
        block = nums[i:i + n]
        cipher_block = np.dot(K, block) % 26
        res.extend([chr(val + ord('A')) for val in cipher_block])

    return "".join(res)

def decryp(key, text):
    K = np.array(key)
    n = len(K)
    
    # 1. Determinant and its modular inverse
    det = int(round(np.linalg.det(K))) % 26
    det_inv = pow(det, -1, 26)  # Python 3.8+ built-in modular inverse

    # Adjugate matrix mod 26: adj(K) = det * K^(-1)
    K_inv = np.round(np.linalg.inv(K) * np.linalg.det(K)).astype(int)
    K_inv = (det_inv * K_inv) % 26

    # 3. Text to numbers
    nums = [ord(c) - ord('A') for c in text]

    # 4. Divide, multiply by inverse matrix, and collect
    res = []
    for i in range(0, len(nums), n):
        block = nums[i:i + n]
        plain_block = np.dot(K_inv, block) % 26
        res.extend([chr(val + ord('A')) for val in plain_block])

    return "".join(res)

key = [[3,3],
       [2,5]]
text = "Hello its Nishanka"

cipher = encryp(key,text)
print("The Cipher text is " + cipher)
print("The plain text after Decryption is "+ decryp(key,cipher))