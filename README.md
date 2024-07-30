# cbase - Library of C Data Structures and Tests
This project contains a collection of useful data structures that are set up in
an almost object-oriented manner.

I built this project to set up an environment for future C projects and provide
a long-term project that I could work on indefinitely, as more can always be
improved.

## Table of Contents
1. [Overview](#overview)
    * [Public](#public)
    * [Private](#private)
    * [Test](#test)
2. [Usage](#usage)
    * [Basic Usage](#basic-usage)
    * [Make Targets](#make-targets)
    * [Executables](#executables)
3. [Contribution](#contribution)
4. [License](#license)
5. [Credits](#credits)

## Overview
The layout of this project is simple: inside [`src/`](/src/), there are folders
for each data structure, which are laid out as follows:

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

It is highly advised that the definition of a data structure's structs be inside
a private header file so that it may not be edited externally.

### Private
The private folder should contain all the C files necessary for the data
structure's methods and any internal header files needed for passing information
between the private C files, such as struct definition.

### Test
The test folder should contain any C or header files necessary for testing the
data structure.

Any function which is itself a test function should be marked with the
`__attribute__((test))` attribute on the same line as the function name.

NOTE: all files in the test folder will only be excluded from the main build and
will only be included for the test build.

## Usage
The project must first be built with `make` before it can be ran.

### Basic Usage
```
make        # Build main and test executables
./bin/test  # Run all the tests
./bin/main  # Run the main program
```

### Make Targets
 - `make all` (default) targets build and test
 - `make build` make the main executable in `bin/main`
 - `make test` make the test executable in `bin/test` and each test file's
    executable inside `bin/tests/`
 - `make clean` removes any files/folders produced from make targets
 - `make debug` make build and test executables with debug symbols
 - `make coverage` make html coverage report inside `coverage/`
 - `make tar` create `cbase.tar.gz` with any project files

### Executables
All executables will be located inside `bin/` with the folder structure of:
 - `bin/`
    - `tests/` All the test executables for each test file
    - `main` The main program executable
    - `test` The test program to run each test file executable

## Contribution
For information on the guidelines and rules for contributing to cbase, please
visit the [`CONTRIBUTION.md`](/CONTRIBUTION.md)

## License
This project is protected under the [GNU General Public License](/LICENSE).

## Credits
 * [Isaac Denning](http://github.com/idenning2003)
