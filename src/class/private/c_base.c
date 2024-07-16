#include <stddef.h>

#include "class.h"

/**
 * @brief Returns the identifier of specific class
 *
 * @param c The class
 * @return const char* The identifier of the class
 */
const char* class_identifier(const class_t* c) {
  return c->identifier;
}

/**
 * @brief Destroys an object of this class type
 *
 * @param c The class
 * @param self the object to destroy
 */
void class_destroy(const class_t* c, void* self) {
  if (c->destroy != NULL)
    c->destroy(self);
}

/**
 * @brief Returns a representation of an object of this class type
 *
 * @param c The class
 * @param self the object to represent
 * @return rope_t* Representation of the object
 *
 * @note The returned rope must be destroyed after use
 * @note Returns NULL if no representation function is defined
 */
rope_t* class_repr(const class_t* c, const void* self) {
  if (c->repr == NULL)
    return NULL;
  return c->repr(self);
}

/**
 * @brief Returns a hash of an object of this class type
 *
 * @param c The class
 * @param self The object to hash
 * @return uint64_t The hash of the object
 *
 * @note Returns pointer hash if no hashing function is defined
 */
uint64_t class_hash(const class_t* c, const void* self) {
  if (c->hash == NULL)
    return class_hash(ptr_class, self);
  return c->hash(self);
}

/**
 * @brief Returns the comparison of two object of this class type
 *
 * @param c The class
 * @param self The first object to compare
 * @param other The second object to compare
 * @return int
 *    >0: self > other
 *    <0: self < other
 *    =0: self = other
 *
 * @note Returns pointer comparison if no comparison function is defined
 */
int class_cmp(const class_t* c, const void* self, const void* other) {
  if (c->hash == NULL)
    class_cmp(ptr_class, self, other);
  return c->cmp(self, other);
}
