my-cplusplus-playground
==============

This is my C++ playground.

Some code here are just examples or exercises for C++ features and some code are
really useful and serious.

Most of this code was developed and compiled with GCC on Linux and Cygwin.
For the samples without a Makefile, compile with the command:
``` shell
 g++ -Wall -std=c++1y <INPUT.cpp> -o <OUTPUT>
```

* `curry/` - [Currying](https://en.wikipedia.org/wiki/Currying) of functions and function-like types.
* `nosj-cpp/` - A JSON library that works with UTF-8-encoded strings.
* `SI/` - Types and operations on physical units - A toy-project to practice
meta-programming with C++ templates. (You should not use this in serious applications!
I recommend using Boost.Units library instead.)
* `static-strings/` - Implementation of compile-time strings with some operations
like concatenation, substrings, etc. Based on an idea found at
[Stack Overflow](http://stackoverflow.com/a/15863804/747919).
* `typedecl/` - Exercises template specialization to detect every kind of type
and generate a string representation of them.
* `unified-assignment.cpp` - An example for the unified assignment that acts as
both copy and move assignment. For pre-C++11 code, it allows copy elision if used
instead of a regular copy assignment that takes a const reference. See
[copy-and-swap idiom](http://en.wikibooks.org/wiki/More_C++_Idioms/Copy-and-swap).
* `user-defined-literals.cpp` - Includes the definition of a compile-time binary
number literal.


References
----------
* Wikipedia [C++11](http://en.wikipedia.org/wiki/C++11),
            [C++14](http://en.wikipedia.org/wiki/C++14) and
            [C++17](http://en.wikipedia.org/wiki/C++17) articles
* http://cppreference.com/
* http://www.cplusplus.com/reference/
* [C++ Idioms](http://en.wikibooks.org/wiki/More C++ Idioms#Table_of_Contents)
* [C++ Standard Drafts](https://github.com/cplusplus/draft)
