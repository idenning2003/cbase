#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>

#include "global.h"
#include "list.h"
#include "l_internal.h"
#include "type.h"

/**
 * @brief Allocates and sets up list
 *
 * @param type The type of structures contained in the list
 * @param connect_destroy when the object is destroyed, should the data be
 * @return list_t* The set up list.
 *
 * @warning If returns NULL, allocation failed
 */
list_t* list_create(const type_t* type, bool connect_destroy) {
  list_t* self = (list_t*)malloc(sizeof(*_self));
  _type = type;
  if (_type == NULL)
    _type = ptr_type;
  _head.next = &_tail;
  _head.prev = NULL;
  _head.data = NULL;
  _tail.prev = &_head;
  _tail.next = NULL;
  _tail.data = NULL;
  _iter = &_head;
  _index = -1;
  _size = 0;
  _connect_destroy = connect_destroy;
  _ordered = false;
  _reversed = false;
  return (list_t*)_self;
}

/**
 * @brief Takes down and frees list
 *
 * @param self The list
 * @return uint8_t Status code
 *    [EXIT_SUCCESS]
 */
void list_destroy(list_t* self) {
  list_clear(self);
  free(_self);
}

/**
 * @brief Creates a copy of this list
 *
 * @param self The list
 * @return list_t* The copy
 */
list_t* list_copy(const list_t* self) {
  list_t* other = list_create(_type, _connect_destroy);
  __list_node_t* n = &_head;
  while ((n = n->next) != &_tail)
    list_append(other, type_copy(_type, n->data));
  if (_reversed)
    list_reverse(other);
  if (_ordered)
    list_order(other);
  return other;
}

/**
 * @brief The number of items are in the list
 *
 * @param self The list
 * @return size_t The number of items in the list
 */
size_t list_size(const list_t* self) {
  return _size;
}

/**
 * @brief Moves the list iterator to the head
 *
 * @param self The list
 * @return uint8_t Status code
 *    [EXIT_SUCCESS]
 */
uint8_t list_head(list_t* self) {
  _iter = &_head;
  _index = -1;
  return EXIT_SUCCESS;
}

/**
 * @brief Moves the list iterator to the tail
 *
 * @param self The list
 * @return uint8_t Status code
 *    [EXIT_SUCCESS]
 */
uint8_t list_tail(list_t* self) {
  _iter = &_tail;
  _index = _size;
  return EXIT_SUCCESS;
}

/**
 * @brief Moves the list iterator to the specified index
 *
 * @param self The list
 * @param index The index to go to
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ERANGE]
 */
uint8_t list_goto(list_t* self, size_t index) {
  if (index >= _size)
    return ERANGE;
  if (_iter == &_head)
    list_next(self, NULL);
  while (_index < index) {
    list_next(self, NULL);
  }
  while (_index > index) {
    list_prev(self, NULL);
  }
  return EXIT_SUCCESS;
}

/**
 * @brief Retrieves the item from the list at specified index and
 *
 * @param self The list
 * @param item Out pointer for the item retrieved
 * @param index The index of item to retrieve
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ERANGE]
 */
uint8_t list_at(const list_t* self, list_item_t** item, size_t index) {
  __list_node_t* n = _head.next;
  size_t index2 = 0;
  if (index >= _size)
    return ERANGE;

  // If the iterator is closer jump there.
  if (_index <= index) {
    n = _iter;
    index2 = _index;
  }

  // Move forward to the item
  while (index2++ < index)
    n = n->next;
  *item = n->data;
  return EXIT_SUCCESS;
}

/**
 * @brief Retrieves the next item from the list and
 * @brief increments the list iterator
 *
 * @param self The list
 * @param item Out pointer for the item retrieved
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ERANGE]
 */
uint8_t list_next(list_t* self, list_item_t** item) {
  if (!list_has_next(self))
    return ERANGE;
  _iter = _iter->next;
  _index++;
  if (item != NULL)
    *item = _iter->data;
  return EXIT_SUCCESS;
}

/**
 * @brief Retrieves the previous item from the list and
 * @brief decrements the list iterator
 *
 * @param self The list
 * @param item Out pointer for the item retrieved
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ERANGE]
 */
uint8_t list_prev(list_t* self, list_item_t** item) {
  if (!list_has_prev(self))
    return ERANGE;
  _iter = _iter->prev;
  _index--;
  if (item != NULL)
    *item = _iter->data;
  return EXIT_SUCCESS;
}

/**
 * @brief If list has next item retrievable
 *
 * @param self The list
 * @return true
 * @return false
 */
bool list_has_next(const list_t* self) {
  return _iter != &_tail && _iter->next != &_tail;
}

/**
 * @brief If list has next item retrievable
 *
 * @param self The list
 * @return true
 * @return false
 */
bool list_has_prev(const list_t* self) {
  return _iter != &_head && _iter->prev != &_head;
}

/**
 * @brief Append an item to the end of the list
 *
 * @param self The list
 * @param item The item to append
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ENOMEM, ENOTSUP, *ERANGE]
 * @warning Append is not allowed for ordered lists, instead use insert
 */
uint8_t list_append(list_t* self, list_item_t* item) {
  if (_ordered)
    return ENOTSUP;
  return list_place(self, item, _size);
}

/**
 * @brief Places an item at specified index of the list
 *
 * @param self The list
 * @param item The item to place
 * @param index The index to place item at
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ENOMEM, ENOTSUP, ERANGE]
 * @warning Place is not allowed for ordered lists, instead use insert
 */
uint8_t list_place(list_t* self, list_item_t* item, size_t index) {
  if (_ordered)
    return ENOTSUP;
  uint8_t err;
  if (index == _size) {
    list_tail(self);
  } else {
    if ((err = list_goto(self, index)))
      return err;
  }
  return __list_node_insert(self, item);
}

/**
 * @brief Inserts an item into the list
 * @note For unordered lists item is placed at iterator
 *
 * @param self The list
 * @param item The item to insert
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ERANGE, ENOMEM]
 */
uint8_t list_insert(list_t* self, list_item_t* item) {
  if (_ordered && _size > 0) {
    list_item_t* item2 = NULL;
    list_head(self);
    list_next(self, &item2);
    while (
      (!_reversed && type_cmp(_type, item, item2) > 0) ||
      (_reversed && type_cmp(_type, item, item2) < 0)
    ) {
      if (list_next(self, &item2)) {
        list_tail(self);
        break;
      }
    }
  }
  return __list_node_insert(self, item);
}

/**
 * @brief Deletes all items from the list
 *
 * @param self The list
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, *ERANGE]
 */
uint8_t list_clear(list_t* self) {
  while (!list_delete(self, 0));
  return EXIT_SUCCESS;
}

/**
 * @brief Delete item a specified index from the list
 *
 * @param self The list
 * @param index The index of the item to delete
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ERANGE]
 */
uint8_t list_delete(list_t* self, size_t index) {
  uint8_t err;
  if ((err = list_goto(self, index)))
    return err;
  return __list_node_delete(self, _iter);
}

/**
 * @brief Removes first instance of item from the list
 *
 * @param self The list
 * @param item The item to remove from the list
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ENOENT, *ERANGE]
 */
uint8_t list_remove(list_t* self, const list_item_t* item) {
  list_item_t* item2;
  list_head(self);
  while (!list_next(self, &item2)) {
    if (!type_cmp(_type, item, item2)) {
      __list_node_delete(self, _iter);
      return EXIT_SUCCESS;
    } else if (
      _ordered &&
      ((!_reversed && type_cmp(_type, item, item2) < 0) ||
      (_reversed && type_cmp(_type, item, item2) > 0))
    ) {
      break;
    }
  }
  return ENOENT;
}

/**
 * @brief Removes all instances of item from the list
 *
 * @param self The list
 * @param item The item to remove from the list
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ENOENT, *ERANGE]
 */
uint8_t list_purge(list_t* self, const list_item_t* item) {
  bool found = false;
  list_head(self);
  while (!list_next(self, NULL)) {
    while (_iter != &_tail && !type_cmp(_type, item, _iter->data)) {
      __list_node_delete(self, _iter);
      found = true;
    }
    if (
      _ordered &&
      ((!_reversed && type_cmp(_type, item, _iter->data) < 0) ||
      (_reversed && type_cmp(_type, item, _iter->data) > 0))
    ) {
      break;
    }
  }
  if (!found)
    return ENOENT;
  return EXIT_SUCCESS;
}

/**
 * @brief Reverses the order of the items in the list
 *
 * @param self The list
 * @return uint8_t Status code
 *    [EXIT_SUCCESS]
 */
uint8_t list_reverse(list_t* self) {
  __list_node_t* n1 = &_head;
  __list_node_t* n2 = &_tail;
  while (n1 != n2 && n2->next != n1) {
    list_item_t* temp;
    temp = n1->data;
    n1->data = n2->data;
    n2->data = temp;
    n1 = n1->next;
    n2 = n2->prev;
  }
  _reversed = !_reversed;
  return EXIT_SUCCESS;
}

/**
 * @brief Orders the items of the list and redefines the list as ordered
 *
 * @param self The list
 * @return uint8_t Status code
 *    [EXIT_SUCCESS]
 */
uint8_t list_order(list_t* self) {
  _ordered = true;
  __list_node_t* n = &_head;
  while ((n = n->next) != &_tail && n->next != &_tail) {
    if (
      (_reversed && type_cmp(_type, n->data, n->next->data) < 0) ||
      (!_reversed && type_cmp(_type, n->data, n->next->data) > 0)
    ) {
      void* tmp = n->data;
      n->data = n->next->data;
      n->next->data = tmp;
      if (n->prev != &_head)
        n = n->prev->prev;
    }
  }
  return EXIT_SUCCESS;
}

/**
 * @brief Redefine the current list as unordered
 *
 * @param self The list
 * @return uint8_t Status code
 *    [EXIT_SUCCESS]
 */
uint8_t list_unorder(list_t* self) {
  _ordered = false;
  return EXIT_SUCCESS;
}

/**
 * @brief Finds the index of the first instance of specified item in the list
 *
 * @param self The list
 * @param index Out pointer for the index
 * @param item the item to find
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ENOENT]
 */
uint8_t list_indexof(
  const list_t* self,
  size_t* index,
  const list_item_t* item
) {
  __list_node_t* n = &_head;
  size_t index2 = 0;
  while ((n = n->next) != &_tail) {
    if (!type_cmp(_type, item, n->data)) {
      if (index != NULL)
        *index = index2;
      return EXIT_SUCCESS;
    } else if (
      _ordered &&
      ((!_reversed && type_cmp(_type, item, n->data) < 0) ||
      (_reversed && type_cmp(_type, item, n->data) > 0))
    ) {
      break;
    }
    index2++;
  }
  return ENOENT;
}

/**
 * @brief If the list contains the specified item
 *
 * @param self The list
 * @param item The item to check for
 * @return true
 * @return false
 */
bool list_contains(const list_t* self, const list_item_t* item) {
  return list_indexof(self, NULL, item) == EXIT_SUCCESS;
}

/**
 * @brief Compares two lists
 *
 * @param self The list
 * @param other The list to compare to
 * @return int
 *    >0: self > other
 *    <0: self < other
 *    =0: self = other
 */
int list_cmp(const list_t* self, const list_t* other) {
  __list_node_t* n1 = &_head;
  __list_node_t* n2 = &(((__list_t*)other)->head);
  int cmp = 0;
  const type_t* type = _type;
  if (type != (((__list_t*)other)->type))
    type = ptr_type;
  while (
    (n1 = n1->next) != &_tail &&
    (n2 = n2->next) != &(((__list_t*)other)->tail) &&
    !(cmp = type_cmp(type, n1->data, n2->data))
  );
  if (n1 == &_tail && n2->next != &(((__list_t*)other)->tail))
    cmp = -1;
  else if (n2 == &(((__list_t*)other)->tail))
    cmp = 1;
  return cmp;
}

/**
 * @brief Returns the type with this list
 *
 * @param self The list
 * @return const type_t* The type with this list
 */
const type_t* list_typeof(const list_t* self) {
  return _type;
}

/**
 * @brief Inserts item node at the current iterator location
 *
 * @param self The list
 * @param item The item to insert
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ENOMEM]
 */
uint8_t __list_node_insert(list_t* self, list_item_t* item) {
  __list_node_t* n = (__list_node_t*)malloc(sizeof(*n));
  if (_iter == &_head) {
    if (_size > 0)
      list_next(self, NULL);
    else
      list_tail(self);
  }
  n->data = item;
  n->next = _iter;
  n->prev = _iter->prev;
  n->next->prev = n;
  n->prev->next = n;
  _iter = n;
  _size++;
  return EXIT_SUCCESS;
}

/**
 * @brief Deletes specified node from the list
 *
 * @param self The list
 * @param n The node to delete
 * @return uint8_t Status code
 *    [EXIT_SUCCESS]
 */
uint8_t __list_node_delete(list_t* self, __list_node_t* n) {
  n->prev->next = n->next;
  n->next->prev = n->prev;
  _size--;
  _iter = n->next;
  if (_connect_destroy)
    type_destroy(_type, n->data);
  free(n);
  return EXIT_SUCCESS;
}

/**
 * @brief Prints the list to stdout
 *
 * @param f The file to write to
 * @param self The list
 */
void __list_print(FILE* f, const list_t* self) {
  rope_t* rope = type_repr(list_type, self);
  char* str = rope_str(rope);
  fprintf(f, "%s\n", str);
  free(str);
  rope_destroy(rope);
}
