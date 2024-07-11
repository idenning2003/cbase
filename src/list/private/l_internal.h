#ifndef L_INTERNAL_H
#define L_INTERNAL_H

#include <stddef.h>

#include "text.h"

#define _l ((__list_t*)l)
#define _head (_l->head)
#define _tail (_l->tail)
#define _iter (_l->iter)
#define _index (_l->index)
#define _size (_l->size)
#define _item_destroy_func (_l->item_destroy_func)
#define _item_cmp_func (_l->item_cmp_func)
#define _item_totext_func (_l->item_totext_func)
#define _reversed (_l->reversed)

typedef void list_item_t;
struct list_node_struct;
typedef struct list_node_struct __list_node_t;

uint8_t __list_node_insert(list_t*, list_item_t*);
uint8_t __list_node_delete(list_t*, __list_node_t*);
int __list_item_cmp(
  const list_t*,
  const list_item_t*,
  const list_item_t*
);
text_t* __ptr_totext(const void*);

struct list_node_struct {
  list_item_t* data;
  __list_node_t* next;
  __list_node_t* prev;
};

typedef struct {
  size_t size;
  size_t index;
  void (*item_destroy_func)(list_item_t*);
  int (*item_cmp_func)(const list_item_t*, const list_item_t*);
  text_t* (*item_totext_func)(const list_item_t*);
  __list_node_t head;
  __list_node_t tail;
  __list_node_t* iter;
  bool reversed;
} __list_t;

#endif
