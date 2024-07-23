#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "l_internal.h"
#include "type.h"

/**
 * @brief Returns the representation of this list
 *
 * @param self The list
 * @return rope_t* The representation of this list
 */
rope_t* list_repr(const list_t* self) {
  rope_t* rope, *temp;
  __list_node_t* n = &_head;
  rope = rope_create("[");
  while ((n = n->next) != &_tail) {
    temp = type_repr(_type, n->data);
    rope_add(rope, temp);
    if (n->next != &_tail)
      rope_concat(rope, ", ");
    rope_destroy(temp);
  }
  rope_append(rope, ']');

  return rope;
}

/**
 * @brief Return a hash of this list using a modified djb2
 *
 * @param self The string
 * @return uint64_t The hash of this string
 */
uint64_t list_hash(const list_t* self) {
  uint64_t hash = 5381;
  __list_node_t* n = &_head;
  while ((n = n->next) != &_tail)
    hash = ((hash << 5) + hash) + type_hash(_type, n->data);

  return hash;
}

/// @brief List type
const type_t* list_type = &(type_t){
  .identifier = "list",
  .destroy = list_destroy,
  .repr = list_repr,
  .hash = list_hash,
  .cmp = list_cmp
};
