def num_even_digits(n: int) -> int:
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
      n = abs(n)
      return sum(1 for ch in str(n) if int(ch) % 2 == 0)
if __name__ == '__main__':
    import doctest
    doctest.testmod()
