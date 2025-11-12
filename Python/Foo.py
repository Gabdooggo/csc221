class Foo:
    """
      >>> foo = Foo(42, 'eggs')
      >>> foo.num
      42
      >>> foo.food
      'eggs'
      >>> foo.message()
      'I love eggs!'
    """
    
    def __init__(self, num, food):
        self.num = num
        self.food = food

    def message(self):
       return f'I love {self.food}!'
if __name__ == '__main__':
    import doctest
    doctest.testmod()
