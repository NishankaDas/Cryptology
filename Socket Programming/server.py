# server.py
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

shift = 3  # shared key

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(("0.0.0.0", 12345))
server.listen(1)

print("Server waiting for connection...")
conn, addr = server.accept()
print(f"Connected to {addr}")

while True:
    encrypted_msg = conn.recv(1024).decode()
    if not encrypted_msg:
        break

    print("Client (encrypted):", encrypted_msg)
    print("Client (decrypted):", decrypt(encrypted_msg, shift))

    msg = input("You: ")
    encrypted = encrypt(msg, shift)
    conn.send(encrypted.encode())

conn.close()
