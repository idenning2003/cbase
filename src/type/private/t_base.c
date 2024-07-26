#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "type.h"

/**
 * @brief Returns the identifier of specific type
 *
 * @param t The type
 * @return const char* The identifier of the type
 */
const char* type_identifier(const type_t* t) {
  return t->identifier;
}

/**
 * @brief Destroys an object of this type
 *
 * @param t The type
 * @param self The object to destroy
 */
void type_destroy(const type_t* t, void* self) {
  if (t->destroy != NULL)
    t->destroy(self);
}

/**
 * @brief Returns the comparison of two object of this type
 *
 * @param t The type
 * @param self The object to copy
 * @return void* The copy of the object
 */
void* type_copy(const type_t* t, const void* self) {
  if (t->copy == NULL)
    return type_copy(ptr_type, self);
  return t->copy(self);
}

/**
 * @brief Returns a representation of an object of this type
 *
 * @param t The type
 * @param self The object to represent
 * @return rope_t* Representation of the object
 *
 * @note The returned rope must be destroyed after use
 * @note Returns NULL if no representation function is defined
 */
rope_t* type_repr(const type_t* t, const void* self) {
  if (t->repr == NULL)
    return type_repr(ptr_type, self);
  return t->repr(self);
}

/**
 * @brief Returns a hash of an object of this type
 *
 * @param t The type
 * @param self The object to hash
 * @return uint64_t The hash of the object
 *
 * @note Returns pointer hash if no hashing function is defined
 */
uint64_t type_hash(const type_t* t, const void* self) {
  if (t->hash == NULL)
    return type_hash(ptr_type, self);
  return t->hash(self);
}

/**
 * @brief Returns the comparison of two object of this type
 *
 * @param t The type
 * @param self The first object to compare
 * @param other The second object to compare
 * @return int
 *    >0: self > other
 *    <0: self < other
 *    =0: self = other
 *
 * @note Returns pointer comparison if no comparison function is defined
 */
int type_cmp(const type_t* t, const void* self, const void* other) {
  if (t->cmp == NULL)
    return type_cmp(ptr_type, self, other);
  return t->cmp(self, other);
}

/**
 * @brief Prints an object of this type
 *
 * @param f File to write to
 * @param t The type
 * @param self The object to print
 */
void __type_print(FILE* f, const type_t* t, const void* self) {
  rope_t* rope = type_repr(t, self);
  char* repr = rope_str(rope);
  fprintf(f, "%s\n", repr);
  free(repr);
  rope_destroy(rope);
}
