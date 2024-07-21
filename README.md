# cbase - Library of C data structures and tests
This project contains a collection of useful data structures that are set up in
an almost object-oriented manner.

I built this project to set up an environment for future C projects and provide
a long-term project that I could work on indefinitely, as more can always be
improved.

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

## Layout
The layout of this is simple: inside [`src/`](/src/), there are folders for each
data structure, which are laid out as follows:

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

## Contribution Rules
A few rules must be followed under all circumstances to maintain code
consistency and readability across the codebase. As this codebase improves,
global testing will automatically check these rules. However, it is always
better practice to design the code with these rules in mind in the first place.

### 1. Follow The Layout
Conform to the file and folder structure described in the
[layout section](##Layout).

### 2. Invalid Characters
Certain characters are prohibited in the code, such as the tab character `\t` or
the carriage return `\r`. Any non-ASCII character will also not be allowed.

### 3. Column Length
Any file within [`src/`](/src/) must follow the
[80-column rule](https://www.emacswiki.org/emacs/EightyColumnRule) specifies
that every line must have, at most, 80 characters before a new line. All files
outside this scope should still be incentivized to conform to this rule, but it
will not be enforced.

### 4. Newline EOF
Before the end of any file, there must be at least one newline character `\n`.

### 5. Trailing Whitespace
Any trailing whitespace, meaning space characters `' '` before a newline `\n`,
is strictly prohibited. I ([idenning2003](https://github.com/idenning2003)) have
a personal vendetta against trailing whitespace.

### 6. Constant Qualifier
Use it whenever possible in function parameters and whenever it makes sense in
return types.

### 7. Function Headers
All function definitions except for test functions marked with
`__attribute__((test))` must be tagged with a doxygen comment containing a brief
of what the function is for, all the parameters in order with a description, and
the return type with a description. Any extra information that you believe could
be useful for implementation should also be included.

Example from [`src/list/private/l_base.c`](/src/list/private/l_base.c)
```
/**
 * @brief Allocates and sets up list
 *
 * @param type The type of structures contained in the list
 * @param connect_destroy when the object is destroyed, should the data be
 * @return list_t* The set up list.
 *
 * @warning If returns NULL, allocation failed
 */
list_t* list_create(const type_t* type, bool connect_destroy) {
 ...
}
```

### 8. File Headers
All public header files must contain the filename, author, brief of the
file/structure, and date. If the file has been significantly changed, the date
should be updated; however, this is not a hard-set rule.

Example from [`src/list/public/list.h`](/src/list/public/list.h)
```
/**
 * @file list.h
 * @author Isaac Denning (117934828+idenning2003@users.noreply.github.com)
 * @brief Definition of functions for the use of a doubly linked list with
 * sentinel nodes and an iterator.
 * @date 2024-07-11
 */
```

### 9. Scope Control
Opening brackets must be placed on the same line as the begining of the scope
definition. The only exception to this rule is when there is too much logic in
the parenthesis to comply with this rule and the
[80-column rule](https://www.emacswiki.org/emacs/EightyColumnRule). In those
instances, the logic within the parenthesis needs to be separated into different
lines.

![](https://placehold.co/1/7CFC00/7CFC00.png) Acceptable:
```
void hello_world() {
 ...
}
```

![](https://placehold.co/1/7CFC00/7CFC00.png) Acceptable:
```
void hello_world(
 unint8_t param1,
 unint16_t param2,
 unint32_t param3,
 unint64_t param4,
) {
 ...
}
```

![](https://placehold.co/1/F03C15/F03C15.png) NOT Acceptable:
```
void hello_world ()
{
 ...
}
```

Scoped statements such as if, else, while, etc., can always be scoped with
brackets; however, in some instances, scoping to the immediate next line without
brackets is acceptable.

![](https://placehold.co/1/7CFC00/7CFC00.png) Acceptable:
```
if (is_true) {
 do_thing();
}
```

![](https://placehold.co/1/7CFC00/7CFC00.png) Acceptable:
```
if (is_true)
 do_thing();
```

![](https://placehold.co/1/7CFC00/7CFC00.png) Acceptable:
```
if (is_true) {
 do_thing();
} else {
 do_something_else();
}
```

![](https://placehold.co/1/7CFC00/7CFC00.png) Acceptable:
```
if (is_true)
 do_thing();
else
 do_something_else();
```

![](https://placehold.co/1/F03C15/F03C15.png) NOT Acceptable: Scoped code must
not be on the same line.
```
if (is_true) do_something();
```

![](https://placehold.co/1/F03C15/F03C15.png) NOT Acceptable: Scoped code must
be tabbed out.
```
if (is_true)
do_something();
```

![](https://placehold.co/1/F03C15/F03C15.png) NOT Acceptable: Unscoped code must
be untabbed (`do_something_else` is not evaluated within the if statement).
```
if (is_true)
 do_something();
 do_something_else();
```

![](https://placehold.co/1/F03C15/F03C15.png) NOT Acceptable: Bracket usage must
be consistent between connected if and else statements.
```
if (is_true)
 do_thing();
else {
 do_something_else();
 do_something_new();
}
```