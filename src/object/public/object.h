/**
 * @file object.h
 * @author Isaac Denning (117934828+idenning2003@users.noreply.github.com)
 * @brief A structure for allowing instances of different types in the same
 * structure.
 * @date 2024-07-16
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>

#include "type.h"

#define object_print(t, self) ({ \
  __object_print(stdout, t, self); \
}) \

typedef void object_t;

extern const type_t* object_type;

object_t* object_create(const type_t*, void*, bool);
void object_destroy(object_t*);
const type_t* object_typeof(const object_t*);
const char* object_identifier(const object_t*);
rope_t* object_repr(const object_t*);
uint64_t object_hash(const object_t*);
int object_cmp(const object_t* self, const object_t* other);
void __object_print(FILE*, const object_t*);

#endif
