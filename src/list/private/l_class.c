#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "l_internal.h"
#include "class.h"

// TODO Use rope concat instead
rope_t* list_repr(const list_t* self) {
  list_t* item_reprs = list_create(str_class, true);
  char* item_repr;
  __list_node_t* n = &_head;
  size_t len = 0;
  while ((n = n->next) != &_tail) {
    rope_t* rope = class_repr(_type, n->data);
    item_repr = rope_str(rope);
    list_append(item_reprs, item_repr);
    len += strlen(item_repr) + 2;
    rope_destroy(rope);
  }

  char* repr_iter, *repr = (char*)malloc(sizeof(*item_repr) * (len + 1));
  repr[0] = '[';
  repr_iter = repr + 1;
  list_head(item_reprs);
  while (!list_next(item_reprs, (void**)&item_repr)) {
    int len2;
    if (list_has_next(item_reprs))
      sprintf(repr_iter, "%s, %n", item_repr, &len2);
    else
      sprintf(repr_iter, "%s]%n", item_repr, &len2);
    repr_iter += len2;
  }
  list_destroy(item_reprs);
  return repr;
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
    hash = ((hash << 5) + hash) + class_hash(_type, n->data);

  return hash;
}

/// @brief List class
const class_t* list_class = &(class_t){
  .identifier = "list",
  .destroy = list_destroy,
  .repr = list_repr,
  .hash = list_hash,
  .cmp = list_cmp
};
