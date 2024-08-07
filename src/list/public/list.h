/**
 * @file list.h
 * @author Isaac Denning (117934828+idenning2003@users.noreply.github.com)
 * @brief Definition of functions for the use of a doubly linked list with
 * sentinel nodes and an iterator.
 * @date 2024-07-11
 */

#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "type.h"

#define list_print(self) ({ \
  __list_print(stdout, self); \
}) \

typedef void list_t;
typedef void list_item_t;

extern const type_t* list_type;

list_t* list_create(const type_t*, bool);
void list_destroy(list_t*);
list_t* list_copy(const list_t*);
size_t list_size(const list_t*);
uint8_t list_head(list_t*);
uint8_t list_tail(list_t*);
uint8_t list_goto(list_t*, size_t);
uint8_t list_at(const list_t*, list_item_t**, size_t);
uint8_t list_next(list_t*, list_item_t**);
uint8_t list_prev(list_t*, list_item_t**);
bool list_has_next(const list_t*);
bool list_has_prev(const list_t*);
uint8_t list_insert(list_t*, list_item_t*);
uint8_t list_append(list_t*, list_item_t*);
uint8_t list_place(list_t*, list_item_t*, size_t);
uint8_t list_clear(list_t*);
uint8_t list_delete(list_t*, size_t);
uint8_t list_remove(list_t*, const list_item_t*);
uint8_t list_purge(list_t*, const list_item_t*);
uint8_t list_reverse(list_t*);
uint8_t list_order(list_t*);
uint8_t list_unorder(list_t*);
uint8_t list_indexof(const list_t*, size_t*, const list_item_t*);
bool list_contains(const list_t*, const list_item_t*);
int list_cmp(const list_t*, const list_t*);
const type_t* list_typeof(const list_t*);

void __list_print(FILE*, const list_t*);

#endif
