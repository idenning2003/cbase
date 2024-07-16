#ifndef S_INTERNAL_H
#define S_INTERNAL_H

#include <stddef.h>
#include <stdint.h>

#include "text.h"

struct Text_Node;
typedef struct Text_Node __text_node_t;

uint8_t __text_back(text_t*);

struct Text_Node {
  __text_node_t* next;
  char data;
};

typedef struct Text {
  size_t length;
  size_t index;
  __text_node_t* front;
  __text_node_t* back;
  __text_node_t* iter;
} __text_t;

#endif
