# client.py
import socket

def encrypt(text, shift):
    result = ""
    for char in text:
        if char.isalpha():
            base = 65 if char.isupper() else 97
            result += chr((ord(char) - base + shift) % 26 + base)
        else:
            result += char
    return result

def decrypt(text, shift):
    return encrypt(text, -shift)

shift = 3  # same key as server

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(("127.0.0.1", 12345))

while True:
    msg = input("You: ")
    encrypted = encrypt(msg, shift)
    client.send(encrypted.encode())

    encrypted_reply = client.recv(1024).decode()
    print("Server (encrypted):", encrypted_reply)
    print("Server (decrypted):", decrypt(encrypted_reply, shift))

client.close()