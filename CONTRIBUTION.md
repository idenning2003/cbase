# Contributing to cbase
The following is a set of guidelines and rules for contributing to cbase. If
there is something not covered by this document, please use your best judgment
or ask in the
[discussions tab](http://github.com/idenning2003/cbase/discussions).

## Table of Contents
1. [Guidelines](#guidelines)
2. [Rules](#rules)
    1. [Follow The Layout](#1-follow-the-layout)
    2. [Invalid Characters](#2-invalid-characters)
    3. [Column Length](#3-column-length)
    4. [Newline EOF](#4-newline-eof)
    5. [Trailing Whitespace](#5-trailing-whitespace)
    6. [Constant Qualifier](#6-constant-qualifier)
    7. [Function Headers](#7-function-headers)
    8. [File Headers](#8-file-headers)
    9. [Scope Control](#9-scope-control)

## Guidelines
Before working on any code, an issue must be in
[cbase project](https://github.com/users/idenning2003/projects/1). If there is
not one there for the specific task you wish to work on, create it!

Issues must be tagged with their priority level and a point estimate for how
long you believe it should take. The values are all relative, so look at other
issues to get an idea on how to mark these.

Now that you have an issue, you can make a branch off of this issue, which will
automatically name the branch it relative to the issue id and name.

Once you have completed the tasks detailed in the issue, create a pull request
and let others review your code before it should be merged. If there is more you
wish to complete, please create another seperate issue. While it may be tempting
to get it done quickly and work on the same branch, in most cases, it is
preferable to seperate out your work.

## Rules
### 1. Follow The Layout
Conform to the file and folder structure described in the overview of the
[README](/README.md#overview).

### 2. Invalid Characters
Certain characters are prohibited in the code, such as the tab character `\t` or
the carriage return `\r`. Any non-ASCII character will also not be allowed.

### 3. Column Length
Any file within [`src/`](/src/) must follow the
[80-column rule](http://www.emacswiki.org/emacs/EightyColumnRule) specifies
that every line must have, at most, 80 characters before a new line. All files
outside this scope should still be incentivized to conform to this rule, but it
will not be enforced.

### 4. Newline EOF
Before the end of any file, there must be at least one newline character `\n`.

### 5. Trailing Whitespace
Any trailing whitespace, meaning space characters `' '` before a newline `\n`,
is strictly prohibited. I ([Isaac Denning](http://github.com/idenning2003)) have
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
[80-column rule](http://www.emacswiki.org/emacs/EightyColumnRule). In those
instances, the logic within the parenthesis needs to be separated into different
lines.

![](http://placehold.co/1/7CFC00/7CFC00.png) Acceptable:
```
void hello_world() {
 ...
}
```

![](http://placehold.co/1/7CFC00/7CFC00.png) Acceptable:
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

![](http://placehold.co/1/F03C15/F03C15.png) NOT Acceptable:
```
void hello_world ()
{
 ...
}
```

Scoped statements such as if, else, while, etc., can always be scoped with
brackets; however, in some instances, scoping to the immediate next line without
brackets is acceptable.

![](http://placehold.co/1/7CFC00/7CFC00.png) Acceptable:
```
if (is_true) {
 do_thing();
}
```

![](http://placehold.co/1/7CFC00/7CFC00.png) Acceptable:
```
if (is_true)
 do_thing();
```

![](http://placehold.co/1/7CFC00/7CFC00.png) Acceptable:
```
if (is_true) {
 do_thing();
} else {
 do_something_else();
}
```

![](http://placehold.co/1/7CFC00/7CFC00.png) Acceptable:
```
if (is_true)
 do_thing();
else
 do_something_else();
```

![](http://placehold.co/1/F03C15/F03C15.png) NOT Acceptable: Scoped code must
not be on the same line.
```
if (is_true) do_something();
```

![](http://placehold.co/1/F03C15/F03C15.png) NOT Acceptable: Scoped code must
be tabbed out.
```
if (is_true)
do_something();
```

![](http://placehold.co/1/F03C15/F03C15.png) NOT Acceptable: Unscoped code must
be untabbed (`do_something_else` is not evaluated within the if statement).
```
if (is_true)
 do_something();
 do_something_else();
```

![](http://placehold.co/1/F03C15/F03C15.png) NOT Acceptable: Bracket usage must
be consistent between connected if and else statements.
```
if (is_true)
 do_thing();
else {
 do_something_else();
 do_something_new();
}
```