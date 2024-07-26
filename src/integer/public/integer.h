/**
 * @file integer.h
 * @author Isaac Denning (117934828+idenning2003@users.noreply.github.com)
 * @brief Definition of functions for the use of a generic sized integer
 * implementation.
 * @date 2024-07-25
 */

#ifndef INTEGER_H
#define INTEGER_H

#include <stdint.h>
#include <stdio.h>

#include "rope.h"

#define integer_print(self) ({ \
  __integer_print(stdout, self); \
}) \

typedef void integer_t;

integer_t* integer_create_int(int64_t);
integer_t* integer_create_str(char*);
integer_t* integer_create_rope(rope_t*);
void integer_destroy(integer_t*);
integer_t* integer_copy(integer_t*);

void integer_add_integer(integer_t*, integer_t*);

void __integer_print(FILE*, integer_t*);

#endif
