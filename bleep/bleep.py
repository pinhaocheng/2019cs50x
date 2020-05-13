import sys
from cs50 import get_string
from sys import argv

words = set()


def main():
    # check for command line inputs
    if len(sys.argv) != 2:
        print("Usage: python bleep.py dictionary")
        exit(1)

    # prompt user input for plaintext
    message = get_string("What message would you like to censor?\n")

    while(load(sys.argv[1])):
        for word in message.split():
            if (check(word)):
                print("*" * len(word), end=" ")
            else:
                print(word, end=" ")
        print()
        break


def load(dictionary):
    # Load dictionary into memory, returning true if successful else false
    file = open(dictionary, "r")
    for line in file:
        words.add(line.rstrip("\n"))
    file.close()
    return True


def check(word):
    return word.lower() in words


if __name__ == "__main__":
    main()
