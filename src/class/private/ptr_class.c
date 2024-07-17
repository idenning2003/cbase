#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "class.h"

/**
 * @brief Return a representation of this pointer in a rope_t
 *
 * @param p The pointer
 * @return rope_t* The representation of this pointer
 */
rope_t* ptr_repr(const void* p) {
  char* str = (char*)malloc(
    sizeof(*str) *
    (sizeof(long long) * 2 - __builtin_clzll((ptrdiff_t)p) / 4 + 5)
  );
  sprintf(str, "\"%p\"", p);
  rope_t* r = rope_create_with(str);
  free(str);
  return r;
}

/**
 * @brief Return a hash of this pointer
 *
 * @param p The pointer
 * @return uint64_t The hash of this pointer
 */
uint64_t ptr_hash(const void* p) {
  return (uint64_t)(ptrdiff_t)p;
}

/**
 * @brief Compare two pointers
 *
 * @param p1 The first pointer to compare
 * @param p2 The second pointer to compare
 * @return int p1 - p2
 */
int ptr_cmp(const void* p1, const void* p2) {
  return p1 - p2;
}

/// @brief Pointer class
const class_t* ptr_class = &(class_t){
  .identifier = "ptr",
  .destroy = NULL,
  .repr = ptr_repr,
  .hash = ptr_hash,
  .cmp = ptr_cmp
};
