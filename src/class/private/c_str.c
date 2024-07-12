#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "class.h"

/**
 * @brief Return a representation of this string in a rope_t
 *
 * @param s The string
 * @return rope_t* The representation of this string
 */
rope_t* str_repr(const void* s) {
  rope_t* r = rope_create_with(s);
  return r;
}

/**
 * @brief Return a hash of this string using djb2
 *
 * @param s The string
 * @return uint64_t The hash of this string
 */
uint64_t str_hash(const void* s) {
  uint64_t hash = 5381;
  char c;
  while ((c = *(const char*)s++))
    hash = ((hash << 5) + hash) + c;

  return hash;
}

/// @brief String class
const class_t* str_class = &(class_t){
  .name = "str",
  .destroy = free,
  .repr = str_repr,
  .hash = str_hash,
  .cmp = (int (*const)(const void *, const void *))strcmp
};
