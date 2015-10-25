libds
=====

Generic data structure library written in C. Includes the following structures:

 * String buffer
 * Dictionary / hash table
 * Array / stack
 * Linked list / queue
 * Generic iterator for container types

## Getting Started
To get started, clone this repository on your computer:

    git clone github.com/chrisrink10/libds
    
You'll need a copy of CMake 3.2 to generate appropriate makefiles for your 
system. For most UNIX-like systems (OS X, Linux, etc.) you should be able
to simply open a command prompt to the cloned directory and type 
`cmake ..` followed by `make` (and `make install` to install the library).

Note that headers will be installed to `/usr/local/include/libds`, so if
you already have headers at that directory, the install process may overwrite
them.

## How To Use
The header files included in the `include/` directory explain how to use
each of the different data structures provided by the library.

If Doxygen is installed on your system, CMake will generate Doxygen files
from the headers in the `include/` directory.

## License
MIT License
