/**
 * @file object.h
 * @author Isaac Denning (117934828+idenning2003@users.noreply.github.com)
 * @brief A structure for allowing instances of different classes in the same
 * structure.
 * @date 2024-07-16
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>

#include "class.h"

typedef void object_t;

extern const class_t* object_class;

object_t* object_create(const class_t*, void*, bool);
void object_destroy(object_t*);
const class_t* object_type(const object_t*);
const char* object_identifier(const object_t*);
rope_t* object_repr(const object_t*);
uint64_t object_hash(const object_t*);
int object_cmp(const object_t* self, const object_t* other);

#endif
