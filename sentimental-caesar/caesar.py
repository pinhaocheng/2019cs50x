import sys
from cs50 import get_string

# check for command line inputs
if len(sys.argv) != 2:
    print("Usage: python caesar.py k")
    exit(1)

# get key from command line
k = int(sys.argv[1])

# prompt user input for plaintext
plaintext = get_string("plaintext: ")

# print display for ciphertext
print("ciphertext:", end=" ")

# iterates over each char in plaintext
for i in plaintext:

    # capital letter
    if i.isupper():
        output = (ord(i) - ord('A') + k) % 26 + ord('A')

    # normal letter
    elif i.islower():
        output = (ord(i) - ord('a') + k) % 26 + ord('a')

    # non alpha
    else:
        output = ord(i)

    # change back from ASCII to alphabet
    cipher = chr(output)

    # print it out
    print(cipher, end="")

# print new line
print()

