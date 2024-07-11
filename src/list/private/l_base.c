#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>

#include "global.h"
#include "list.h"
#include "l_internal.h"
#include "text.h"

/**
 * @brief Allocates and sets up list
 *
 * @param item_destroy_func Function to call when destroying item
 * @param item_cmp_func Function to compare items of the list
 * @param item_totext_func Function to convert item to string
 * @return list_t* The set up list.
 *
 * @warning If returns NULL, allocation failed
 */
list_t* list_create(
  void (*item_destroy_func)(list_item_t*),
  int (*item_cmp_func)(const list_item_t*, const list_item_t*),
  text_t* (*item_totext_func)(const list_item_t*)
) {
  list_t* l = (list_t*)malloc(sizeof(*_l));
  if (_l == NULL)
    return NULL;
  _head.next = &_tail;
  _head.prev = NULL;
  _head.data = NULL;
  _tail.prev = &_head;
  _tail.next = NULL;
  _tail.data = NULL;
  _iter = &_head;
  _index = -1;
  _size = 0;
  _reversed = false;
  _item_destroy_func = item_destroy_func;
  _item_cmp_func = item_cmp_func;
  _item_totext_func = item_totext_func;
  if (_item_totext_func == NULL)
    _item_totext_func = __ptr_totext;
  return (list_t*)_l;
}

/**
 * @brief Takes down and frees list
 *
 * @param l The list
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, *ERANGE]
 */
uint8_t list_destroy(list_t* l) {
  int err;
  if ((err = list_clear(l)))
    return err;
  free(_l);
  return EXIT_SUCCESS;
}

/**
 * @brief The number of items are in the list
 *
 * @param l The list
 * @return size_t The number of items in the list
 */
size_t list_size(const list_t* l) {
  return _size;
}

/**
 * @brief Moves the list iterator to the head
 *
 * @param l The list
 * @return uint8_t Status code
 *    [EXIT_SUCCESS]
 */
uint8_t list_head(list_t* l) {
  _iter = &_head;
  _index = -1;
  return EXIT_SUCCESS;
}

/**
 * @brief Moves the list iterator to the tail
 *
 * @param l The list
 * @return uint8_t Status code
 *    [EXIT_SUCCESS]
 */
uint8_t list_tail(list_t* l) {
  _iter = &_tail;
  _index = _size;
  return EXIT_SUCCESS;
}

/**
 * @brief Moves the list iterator to the specified index
 *
 * @param l The list
 * @param index The index to go to
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ERANGE]
 */
uint8_t list_goto(list_t* l, size_t index) {
  uint8_t err;
  if (index >= _size)
    return ERANGE;
  if (_iter == &_head) {
    if ((err = list_next(l, NULL)))
      return err;
  }
  while (_index < index) {
    if ((err = list_next(l, NULL)))
      return err;
  }
  while (_index > index) {
    if ((err = list_prev(l, NULL)))
      return err;
  }
  return EXIT_SUCCESS;
}

/**
 * @brief Retrieves the item from the list at specified index and
 *
 * @param l The list
 * @param item Out pointer for the item retrieved
 * @param index The index of item to retrieve
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ERANGE]
 */
uint8_t list_at(const list_t* l, list_item_t** item, size_t index) {
  list_node_internal_t* n = _head.next;
  size_t index2 = 0;
  if (index >= _size)
    return ERANGE;

  // If the iterator is closer jump there.
  if (_index <= index) {
    n = _iter;
    index2 = _index;
  }

  // Move forward to the item
  while (index2 < index) {
    if ((n = n->next) == &_tail)
      return ERANGE;
    index2++;
  }
  *item = n->data;
  return EXIT_SUCCESS;
}

/**
 * @brief Retrieves the next item from the list and
 * @brief increments the list iterator
 *
 * @param l The list
 * @param item Out pointer for the item retrieved
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ERANGE]
 */
uint8_t list_next(list_t* l, list_item_t** item) {
  if (!list_has_next(l))
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
 * @param l The list
 * @param item Out pointer for the item retrieved
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ERANGE]
 */
uint8_t list_prev(list_t* l, list_item_t** item) {
  if (!list_has_prev(l))
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
 * @param l The list
 * @return true
 * @return false
 */
bool list_has_next(const list_t* l) {
  return _iter->next != &_tail && _iter != &_tail;
}

/**
 * @brief If list has next item retrievable
 *
 * @param l The list
 * @return true
 * @return false
 */
bool list_has_prev(const list_t* l) {
  return _iter->prev != &_head && _iter != &_head;
}

/**
 * @brief Append an item to the end of the list
 *
 * @param l The list
 * @param item The item to append
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ENOMEM, ENOTSUP, *ERANGE]
 * @warning Append is not allowed for ordered lists, instead use insert
 */
uint8_t list_append(list_t* l, list_item_t* item) {
  if (_item_cmp_func != NULL)
    return ENOTSUP;
  return list_place(l, item, _size);
}

/**
 * @brief Places an item at specified index of the list
 *
 * @param l The list
 * @param item The item to place
 * @param index The index to place item at
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ENOMEM, ENOTSUP, ERANGE]
 * @warning Place is not allowed for ordered lists, instead use insert
 */
uint8_t list_place(list_t* l, list_item_t* item, size_t index) {
  if (_item_cmp_func != NULL)
    return ENOTSUP;
  uint8_t err;
  if (index == _size) {
    if ((err = list_tail(l)))
      return err;
  } else {
    if ((err = list_goto(l, index)))
      return err;
  }
  return __list_node_insert(l, item);
}

/**
 * @brief Inserts an item into the list
 * @details For unordered lists item is placed at iterator
 *
 * @param l The list
 * @param item The item to insert
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ERANGE, ENOMEM]
 */
uint8_t list_insert(list_t* l, list_item_t* item) {
  if (_item_cmp_func && _size > 0) {
    uint8_t err;
    list_item_t* item2;
    if ((err = list_head(l)))
      return err;
    if ((err = list_next(l, &item2)))
      return err;
    while (
      (!_reversed && _item_cmp_func(item, item2) > 0) ||
      (_reversed && _item_cmp_func(item, item2) < 0)
    ) {
      if (list_next(l, &item2)) {
        list_tail(l);
        break;
      }
    }
  }
  return __list_node_insert(l, item);
}

/**
 * @brief Deletes all items from the list
 *
 * @param l The list
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, *ERANGE]
 */
uint8_t list_clear(list_t* l) {
  while (!list_delete(l, 0));
  return EXIT_SUCCESS;
}

/**
 * @brief Delete item a specified index from the list
 *
 * @param l The list
 * @param index The index of the item to delete
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ERANGE]
 */
uint8_t list_delete(list_t* l, size_t index) {
  uint8_t err;
  if ((err = list_goto(l, index)))
    return err;
  return __list_node_delete(l, _iter);
}

/**
 * @brief Removes first instance of item from the list
 *
 * @param l The list
 * @param item The item to remove from the list
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ENOENT, *ERANGE]
 */
uint8_t list_remove(list_t* l, const list_item_t* item) {
  list_item_t* item2;
  uint8_t err;
  if ((err = list_head(l)))
    return err;
  while (!list_next(l, &item2)) {
    if (__list_item_cmp(l, item, item2)) {
      if ((err = __list_node_delete(l, _iter)))
        return err;
      return EXIT_SUCCESS;
    }
  }
  return ENOENT;
}

/**
 * @brief Removes all instances of item from the list
 *
 * @param l The list
 * @param item The item to remove from the list
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ENOENT, *ERANGE]
 */
uint8_t list_purge(list_t* l, const list_item_t* item) {
  list_item_t* item2;
  uint8_t err;
  bool found = false;
  if ((err = list_head(l)))
    return err;
  if ((err = list_next(l, &item2)))
    return err;
  while (_iter != &_tail) {
    if (__list_item_cmp(l, item, item2)) {
      found = true;
      if ((err = __list_node_delete(l, _iter)))
        return err;
      item2 = _iter->data;
    } else {
      if (list_next(l, &item2))
        break;
    }
  }
  if (!found)
    return ENOENT;
  return EXIT_SUCCESS;
}

/**
 * @brief Creates a copy of this list
 *
 * @param l The list
 * @return list_t* The copy
 */
list_t* list_copy(const list_t* l) {
  list_t* l2 = list_create(
    _item_destroy_func,
    _item_cmp_func,
    _item_totext_func
  );
  if (l2 == NULL)
    return NULL;
  list_node_internal_t* n = &_head;
  while ((n = n->next) != &_tail) {
    if (list_append(l2, n->data))
      return NULL;
  }
  return l2;
}

/**
 * @brief Reverses the order of the items in the list
 *
 * @param l The list
 * @return uint8_t Status code
 *    [EXIT_SUCCESS]
 */
uint8_t list_reverse(list_t* l) {
  list_node_internal_t* n1 = &_head;
  list_node_internal_t* n2 = &_tail;
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
 * @brief Redefine the current list as unordered
 *
 * @param l The list
 * @return uint8_t Status code
 *    [EXIT_SUCCESS]
 */
uint8_t list_unorder(list_t* l) {
  _item_cmp_func = NULL;
  _reversed = false;
  return EXIT_SUCCESS;
}

/**
 * @brief Finds the index of the first instance of specified item in the list
 *
 * @param l The list
 * @param index Out pointer for the index
 * @param item the item to find
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ENOENT]
 */
uint8_t list_indexof(const list_t* l, size_t* index, const list_item_t* item) {
  list_node_internal_t* n = &_head;
  size_t index2 = 0;
  while ((n = n->next) != &_tail) {
    if (__list_item_cmp(l, item, n->data)) {
      *index = index2;
      return EXIT_SUCCESS;
    }
    index2++;
  }
  return ENOENT;
}

/**
 * @brief If the list contains the specified item
 *
 * @param l The list
 * @param item The item to check for
 * @return true
 * @return false
 */
bool list_contains(const list_t* l, const list_item_t* item) {
  return list_indexof(l, NULL, item) == EXIT_SUCCESS;
}

/**
 * @brief Compares two lists
 *
 * @param l The list
 * @param l2 The list to compare to
 * @return int
 */
int list_cmp(UNUSED const list_t* l, UNUSED const list_t* item) {
  return ENOSYS;
}

/**
 * @brief Convert the to a json style text object.
 *
 * @param l The list
 * @return text_t* Text object for this list.
 */
text_t* list_totext(const list_t* l) {
  list_node_internal_t* n = &_head;
  text_t* t = text_create();
  text_t* temp;
  text_append(t, '[');
  while ((n = n->next) != &_tail) {
    temp = _item_totext_func(n->data);
    text_concat_text(t, temp);
    text_destroy(temp);
    if (n->next != &_tail)
      text_concat_string(t, ", ");
  }
  text_append(t, ']');
  return t;
}

/**
 * @brief Print this list to the terminal.
 *
 * @param l The list
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ENOMEM, *ERANGE]
 */
uint8_t list_print(const list_t* l) {
  uint8_t err;
  text_t* t = list_totext(l);
  if (t == NULL)
    return ENOMEM;
  char* s = text_tostring(t);
  if (s == NULL)
    return ENOMEM;
  printf("%s",s);
  if ((err = text_destroy(t)))
    return err;
  free(s);
  return EXIT_SUCCESS;
}

/**
 * @brief Inserts item node at the current iterator location
 *
 * @param l The list
 * @param item The item to insert
 * @return uint8_t Status code
 *    [EXIT_SUCCESS, ENOMEM]
 */
uint8_t __list_node_insert(list_t* l, list_item_t* item) {
  list_node_internal_t* n = (list_node_internal_t*)malloc(sizeof(*n));
  if (n == NULL)
    return ENOMEM;
  if (_iter == &_head) {
    if (_size > 0)
      list_next(l, NULL);
    else
      list_tail(l);
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
 * @param l The list
 * @param n The node to delete
 * @return uint8_t Status code
 *    [EXIT_SUCCESS]
 */
uint8_t __list_node_delete(list_t* l, list_node_internal_t* n) {
  n->prev->next = n->next;
  n->next->prev = n->prev;
  _size--;
  _iter = n->next;
  if (_item_destroy_func != NULL)
    _item_destroy_func(n->data);
  free(n);
  return EXIT_SUCCESS;
}

/**
 * @brief Compare two items by the rules of this list
 *
 * @param l The list
 * @param n1 First node
 * @param n2 Second node
 * @return int 0 if same else if different
 */
int __list_item_cmp(
  const list_t* l,
  const list_item_t* item1,
  const list_item_t* item2
) {
  return (_item_cmp_func && !_item_cmp_func(item1, item2)) || item1 == item2;
}

/**
 * @brief Convert a pointer to text
 *
 * @param ptr The pointer
 * @return text_t* The text
 */
text_t* __ptr_totext(const void* ptr) {
  char arr[21];
  text_t* s = text_create();
  sprintf(arr, "%p", ptr);
  text_concat_string(s, arr);
  return s;
}
