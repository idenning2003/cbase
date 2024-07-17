/**
 * @file class.h
 * @author Isaac Denning (117934828+idenning2003@users.noreply.github.com)
 * @brief This describes a few function pointers that can be used for many
 * structures to interact with eachother generically.
 * @date 2024-07-11
 */

#ifndef CLASS_H
#define CLASS_H

#include <stdint.h>

#include "rope.h"

/// @brief Methods of a class
typedef struct Class {
  /// @brief The identifier of this class
  const char* identifier;
  /// @brief Destroys an object of this class type
  void (*const destroy)(void*);
  /// @brief Returns a representation of an object of this class type
  rope_t* (*const repr)(const void*);
  /// @brief Returns a hash of an object of this class type
  uint64_t (*const hash)(const void*);
  /// @brief Returns the comparison of two object of this class type
  int (*const cmp)(const void*, const void*);
} class_t;

extern const class_t* ptr_class;
extern const class_t* str_class;

void class_destroy(const class_t*, void*);
const char* class_identifier(const class_t*);
rope_t* class_repr(const class_t*, const void*);
uint64_t class_hash(const class_t*, const void*);
int class_cmp(const class_t*, const void*, const void*);

#endif
