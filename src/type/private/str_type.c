#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "type.h"

/**
 * @brief Return a copy of this string
 *
 * @param s The string
 * @return void* The copy of this string
 */
void* str_copy(const void* s) {
  char* copy = (char*)malloc(sizeof(*copy) * (strlen(s) + 1));
  strcpy(copy, s);
  return copy;
}

/**
 * @brief Return a representation of this string in a rope_t
 *
 * @param s The string
 * @return rope_t* The representation of this string
 */
rope_t* str_repr(const void* s) {
  char* temp = (char*)malloc(sizeof(*temp) * (strlen(s) + 3));
  sprintf(temp, "\"%s\"", (const char*)s);
  rope_t* r = rope_create_with(temp);
  free(temp);
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

/// @brief String type
const type_t* str_type = &(type_t){
  .identifier = "str",
  .destroy = free,
  .copy = str_copy,
  .repr = str_repr,
  .hash = str_hash,
  .cmp = (int (*const)(const void *, const void *))strcmp
};
