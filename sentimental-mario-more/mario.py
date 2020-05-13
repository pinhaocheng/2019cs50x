from cs50 import get_int

while True:
    x = get_int("Height: ")

    # accept only positive int no greater than 8
    if x > 0 and x < 9:
        break

for i in range(x):
    # printing white spaces on left
    print(" " * (x-i-1), end="")

    # printing left wall
    print("#" * (i + 1), end=" ")

    # printing middle space
    print(" ", end="")

    # printing right wall
    print("#" * (i + 1))
