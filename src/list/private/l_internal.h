#ifndef L_INTERNAL_H
#define L_INTERNAL_H

#include <stddef.h>

#include "text.h"

#define _self ((__list_t*)self)
#define _head (_self->head)
#define _tail (_self->tail)
#define _iter (_self->iter)
#define _index (_self->index)
#define _size (_self->size)
#define _item_destroy_func (_self->item_destroy_func)
#define _item_cmp_func (_self->item_cmp_func)
#define _item_totext_func (_self->item_totext_func)
#define _reversed (_self->reversed)

typedef void list_item_t;
struct List_Node;
typedef struct List_Node __list_node_t;

uint8_t __list_node_insert(list_t*, list_item_t*);
uint8_t __list_node_delete(list_t*, __list_node_t*);
int __list_item_cmp(
  const list_t*,
  const list_item_t*,
  const list_item_t*
);
text_t* __ptr_totext(const void*);

struct List_Node {
  list_item_t* data;
  __list_node_t* next;
  __list_node_t* prev;
};

typedef struct List {
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
