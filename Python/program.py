def do_something(thing):
    for r in range(1, 6):
        for c in range(-1, 3):
            if r == c:
                thing += 2
            else:
                continue
    else:
        thing -= 1
    return thing

print(do_something(5))
