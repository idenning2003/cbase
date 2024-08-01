#ifndef L_INTERNAL_H
#define L_INTERNAL_H

#include <stddef.h>

#define _self             ((__list_t*)self)
#define _head             (_self->head)
#define _tail             (_self->tail)
#define _iter             (_self->iter)
#define _index            (_self->index)
#define _size             (_self->size)
#define _type             (_self->type)
#define _ordered          (_self->ordered)
#define _reversed         (_self->reversed)
#define _connect_destroy  (_self->connect_destroy)

typedef void list_item_t;
struct List_Node;
typedef struct List_Node __list_node_t;

uint8_t __list_node_insert(list_t*, list_item_t*);
uint8_t __list_node_delete(list_t*, __list_node_t*);

struct List_Node {
  list_item_t* data;
  __list_node_t* next;
  __list_node_t* prev;
};

typedef struct List {
  size_t size;
  size_t index;
  __list_node_t head;
  __list_node_t tail;
  __list_node_t* iter;
  const type_t* type;
  bool ordered;
  bool reversed;
  bool connect_destroy;
} __list_t;

#endif
