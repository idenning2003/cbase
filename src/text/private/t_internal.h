#ifndef S_INTERNAL_H
#define S_INTERNAL_H

#include <stddef.h>
#include <stdint.h>

#include "text.h"

struct text_node_struct;
typedef struct text_node_struct text_node_internal_t;

uint8_t _text_back(text_t*);

struct text_node_struct {
  text_node_internal_t* next;
  char data;
};

typedef struct {
  size_t length;
  size_t index;
  text_node_internal_t* front;
  text_node_internal_t* back;
  text_node_internal_t* iter;
} text_internal_t;

#endif
