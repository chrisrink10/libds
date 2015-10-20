libds
=====

Generic data structure library written in C. Includes the following structures:

 * String buffer
 * Dictionary / hash table
 * List / stack

## Getting Started
To get started, clone this repository on your computer:

    git clone github.com/chrisrink10/libds
    
You'll need a copy of CMake 3.1 to generate appropriate makefiles for your 
system. For most UNIX-like systems (OS X, Linux, etc.) you should be able
to simply open a command prompt to the cloned directory and type 
`cmake ..` followed by `make` (and `make install` to install the library).

## How To Use
The header files included in the `include/` directory explain how to use
each of the different data structures provided by the library.

## Caveats
If the library is not compiled with support for Mutexes, then external
synchronization will be required to use the structures in a multi-threaded
environment.

## License
MIT License
