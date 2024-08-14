import sys
from Crypto.Cipher import AES
from os import urandom
import hashlib

KEY = urandom(16)

def pad(s):
    padding_length = AES.block_size - len(s) % AES.block_size
    padding = bytes([padding_length] * padding_length)
    return s + padding

def aesenc(plaintext, key):
    k = hashlib.sha256(key).digest()
    iv = b'\x00' * AES.block_size  # Ensure iv is of type bytes
    plaintext = pad(plaintext)
    cipher = AES.new(k, AES.MODE_CBC, iv)

    return cipher.encrypt(plaintext)

try :
    plaintext = open(sys.argv[1], "rb").read()
except :
    print("File argument needed !! %s <raw payload file>" % sys.argv[0])
    sys.exit()

ciphertext = aesenc(plaintext, KEY)
print("AESKey[] = { 0x" + ", 0x".join(hex(x)[2:] for x in KEY) + ' };')
print("payload[] = { 0x" + ", 0x".join(hex(x)[2:] for x in ciphertext) + ' };') 
