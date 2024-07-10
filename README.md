# cbase - Library of C data structures and tests

This project contains a collection of useful data structures which are setup in
an almost object oriented manner.

I built this project with the intentions of setting up an enviroment for future
C projects as well as provide a long term project that I could work on
indefinitely as more can always be improved.

## Layout

The layout of this is simple, inside [`src/`](/src/) there are folders for each
data structure which are layed out as follows:

 - `<structure>/`
   - `public/`
   - `private/`
   - `test/`
   - `README.md` (optional)

The only files that are allowed to break these rules are
[`src/main.c`](/src/main.c) and [`src/test.c`](/src/test.c).

### Public
The public folder should only contain header files with functions, definitions,
and structs that are strictly necessary for external usage of the data
structure.

It is highly advised that definition of the struct containing the fields of the
data structure be defined inside a private header file so that it may not be
edited externaly.

### Private
The private folder should contain all the C files neccessary for the data
structure's methods as well and any internal header files needed for passing
information between the private C files, such as struct definition.

### Test

The test folder should contain any C or header files neccessary for testing the
data structure.

Any function which is itself a test function should be marked with the
`__attribute__((test))` attribute on the same line as the function name.

NOTE: all files in the test folder will only be excluded from the main build and
will only be included for the test build.
