#include <stdlib.h>

#include "object.h"
#include "o_internal.h"

/**
 * @brief Allocates and sets up object
 *
 * @param type The type of this object
 * @param data Refrence to this ojects data
 * @param connect_destroy when the object is destroyed, should the data be
 * destroyed too?
 * @return object_t* The object
 */
object_t* object_create(
  const type_t* type,
  void* data,
  bool connect_destroy
) {
  object_t* self = (object_t*)malloc(sizeof(*_self));
  _type = type;
  _data = data;
  _connect_destroy = connect_destroy;
  return (object_t*)_self;
}

/**
 * @brief Destroy this object
 *
 * @param self The object
 */
void object_destroy(object_t* self) {
  if (_connect_destroy)
    type_destroy(_type, _data);
  free(_self);
}

/**
 * @brief Returns a copy of this object
 *
 * @param self The object
 * @return void* A copy of this object
 */
object_t* object_copy(const void* self) {
  return object_create(_type, type_copy(_type, _data), true);
}

/**
 * @brief Returns refrence to this ojects data
 *
 * @param self The object
 * @return void* Refrence to this ojects data
 */
void* object_data(const object_t* self) {
  return _data;
}

/**
 * @brief Returns the type of this object
 *
 * @param self The object
 * @return const type_t* The type of this object
 */
const type_t* object_data_type(const object_t* self) {
  return _type;
}

/**
 * @brief Returns the identifier of this object's type
 *
 * @param self The object
 * @return const char* The identifier of this object's type
 */
const char* object_identifier(const object_t* self) {
  return type_identifier(_type);
}

/**
 * @brief Returns the representation of this object
 *
 * @param self The object
 * @return rope_t* The representation of this object
 */
rope_t* object_repr(const object_t* self) {
  return type_repr(_type, _data);
}

/**
 * @brief Returns the hash of this object
 *
 * @param self The object
 * @return uint64_t The hash of this object
 */
uint64_t object_hash(const object_t* self) {
  return type_hash(_type, _data);
}

/**
 * @brief Returns the comparison of this object
 *
 * @param self The object
 * @param other The object to compare to
 * @return int
 *    >0: self > other
 *    <0: self < other
 *    =0: self = other
 *
 * @note if comparison functions are not the same, returns pointer comparison
 */
int object_cmp(const object_t* self, const object_t* other) {
  if (_type->cmp != ((__object_t*)other)->type->cmp)
    return type_cmp(ptr_type, _data, ((__object_t*)other)->data);
  return type_cmp(_type, _data, ((__object_t*)other)->data);
}

/**
 * @brief Prints this object
 *
 * @param f The file to write to
 * @param self The object
 */
void __object_print(FILE* f, const object_t* self) {
  __type_print(f, _type, _data);
}
