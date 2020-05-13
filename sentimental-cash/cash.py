from cs50 import get_float

while True:
    # prompt user for amount of change
    change = get_float("Change owed: ")

    # accept only positive int no greater than 8
    if change > 0:
        break

change = 100 * change

# use all 25 cents possible
coin_used_25 = change // 25
remain = change - 25 * coin_used_25

# use all 10 cents possible
coin_used_10 = remain // 10
remain = remain - 10 * coin_used_10

# use all 5 cents possible
coin_used_5 = remain // 5
remain = remain - 5 * coin_used_5

# remaining are all 1 cents
coin_used = coin_used_25 + coin_used_10 + coin_used_5 + remain

print(int(coin_used))