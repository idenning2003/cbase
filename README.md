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

## Rules
There are a few rules that must be followed under all circumstances in order to
maintain code consistancy and readability across the codebase. As this codebase
improves, these rules will be automatically checked by the global testing.
However, it is always better practice to design the code with these ruiles in
mind in the first place.

### 1. Follow The Layout
Conform to the file and folder structure described in the
[layout section](##Layout).

### 2. Invalid Characters
Chertain characters are not allowed in the code such as the tab character `\t`
or the carriage return `\r`. Any non ascii character will also not be allowed.

### 3. Column Length
Any file within [`src/`](/src/) must follow the
[80 column rule](https://www.emacswiki.org/emacs/EightyColumnRule) specifying
that every line must have, at most, 80 characters before a newline. All files
outside of this scope should still be incentivized to conform to this
rule.

### 4. Newline EOF
Before the end of any file, there must be at least one newline character `\n`.

### 5. Trailing Whitespace
Any trailing whitespace, meaning space characters ' ' before a newline, is
strictly prohibited. I ([idenning2003](https://github.com/idenning2003)) have a
personal vendetta against trailing whitespace.

### 6. Constant Qualifier
Use it whenever it is possible in function parameters and whenever it make sense
in return types.

### 7. Function Headers
All function definitions except for test funtions marked with
`__attribute__((test))` must be tagged with a doxygen comment containing a brief
of what the function is for, all the paramaters in order with a description, and
the return type with a description. Any extra information that you believe could
be useful for implementation should also be included.

Example from [`src/list/private/l_base.c`](/src/list/private/l_base.c)
```
/**
 * @brief Allocates and sets up list
 *
 * @param item_destroy_func Function to call when destroying item
 * @param item_cmp_func Function to compare items of the list
 * @param item_totext_func Function to convert item to string
 * @return list_t* The set up list.
 *
 * @warning If returns NULL, allocation failed
 */
list_t* list_create(
  void (*item_destroy_func)(list_item_t*),
  int (*item_cmp_func)(const list_item_t*, const list_item_t*),
  text_t* (*item_totext_func)(const list_item_t*)
) {
  ...
}
```

### 8. File Headers
All public header files must contain the filename, author, breif of what the
file/structure is, and date. If the file has been significantly changed, then
the date should be updated, however, this is not a hard set rule.

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
definition. The only exeption to this rule is when there is too much logic in
the parenthesis to comply with this rule and the
[80 column rule](https://www.emacswiki.org/emacs/EightyColumnRule). In those
instances, the logic within the parenthesis needs to be seperated to different
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
void hello_world () {
  ...
}
```

Scoped statments such as if, else, while, etc. can always be scoped with
brackets, however, in some instances scoping to the immediate next line without
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

![](https://placehold.co/1/F03C15/F03C15.png) NOT Acceptable: Scoped code must not be on the same line.
```
if (is_true) do_something();
```

![](https://placehold.co/1/F03C15/F03C15.png) NOT Acceptable: Scoped code must be tabbed out.
```
if (is_true)
do_something();
```

![](https://placehold.co/1/F03C15/F03C15.png) NOT Acceptable: Unscoped code must be untabbed (`do_something_else` is not
evaluated within the if statment).
```
if (is_true)
  do_something();
  do_something_else();
```

![](https://placehold.co/1/F03C15/F03C15.png) NOT Acceptable: Bracket usage must be consistent between connected if and else
statements.
```
if (is_true)
  do_thing();
else {
  do_something_else();
  do_something_new();
}
```
