/**
 * @file type.h
 * @author Isaac Denning (117934828+idenning2003@users.noreply.github.com)
 * @brief This describes a few function pointers that can be used for many
 * structures to interact with eachother generically.
 * @date 2024-07-11
 */

#ifndef TYPE_H
#define TYPE_H

#include <stdint.h>

#include "rope.h"

/// @brief Methods of a type
typedef struct Type {
  /// @brief The identifier of this type
  const char* identifier;
  /// @brief Destroys an object of this type
  void (*const destroy)(void*);
  /// @brief Returns a representation of an object of this type
  rope_t* (*const repr)(const void*);
  /// @brief Returns a hash of an object of this type
  uint64_t (*const hash)(const void*);
  /// @brief Returns the comparison of two object of this type
  int (*const cmp)(const void*, const void*);
} type_t;

extern const type_t* ptr_type;
extern const type_t* str_type;

void type_destroy(const type_t*, void*);
const char* type_identifier(const type_t*);
rope_t* type_repr(const type_t*, const void*);
uint64_t type_hash(const type_t*, const void*);
int type_cmp(const type_t*, const void*, const void*);
void type_print(const type_t*, const void*);

#endif
