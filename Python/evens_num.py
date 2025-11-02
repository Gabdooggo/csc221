def num_even_digits(n):
    """
    >>> num_even_digits(123456)
    3
    >>> num_even_digits(2468)
    4
    >>> num_even_digits(1357)
    0
    >>> num_even_digits(2)
    1
    >>> num_even_digits(20)
    2
    """
    n = (n)
    return sum(1 for even in str(n) if int(even) % 2 == 0)

#This first makes n a string so that we can see each number seperately instead of them being together afterwards it makes it an interger again and since we made it a string before the numbers will be seperate and it will take each number and see if it's even by me doing modulus two. If there is no remainder its even and return sum 1 for each digit thats even.

if __name__ == "__main__":
    import doctest
    doctest.testmod()
