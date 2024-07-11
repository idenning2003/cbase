#ifndef S_INTERNAL_H
#define S_INTERNAL_H

#include <stddef.h>
#include <stdint.h>

#include "text.h"

struct text_node_struct;
typedef struct text_node_struct __text_node_t;

uint8_t __text_back(text_t*);

struct text_node_struct {
  __text_node_t* next;
  char data;
};

typedef struct {
  size_t length;
  size_t index;
  __text_node_t* front;
  __text_node_t* back;
  __text_node_t* iter;
} __text_t;

#endif
