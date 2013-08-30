c-plus-plus-11
==============

This is my C++11 playground.

Some code here are just examples or exercises for C++ features and some code are
really useful and serious.

Most of this code was developed and compiled with GCC on Linux and Cygwin.
For the samples without a Makefile, compile with the command:
``` shell
 g++ -Wall -std=c++11 <INPUT.cpp> -o <OUTPUT>
```

* `nosj-cpp/` - A JSON library that works with UTF-8-encoded strings.
* `SI/` - Types and operations on physical units - A toy-project to practice
meta-programming with C++ templates. (You should not use this in serious applications!
I recommend using Boost.Units library instead.)
* `type-names/` - Exercises template specialization to detect every kind of type
and generate a string representation of them.
* `unified-assignment.cpp` - An example for the unified assignment that C++11 enables
as a replacement for the copy-and-swap idiom (See http://en.wikibooks.org/wiki/More_C++_Idioms/Copy-and-swap).
* `user-defined-literals.cpp` - Includes the definition of a compile-time binary
number literal.


References
----------
* [Wikipedia C++11 article](http://en.wikipedia.org/wiki/C++11)
* http://www.cplusplus.com/reference/
* http://cppreference.com/
* [C++ Idioms](http://en.wikibooks.org/wiki/More C++ Idioms#Table_of_Contents)
* [Bjarne Stroustrup's homepage](http://www.stroustrup.com/)
* [Stroustrup's C++11 FAQ](http://www.stroustrup.com/C++11FAQ.html)
