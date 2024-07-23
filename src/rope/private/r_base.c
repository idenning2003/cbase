#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "rope.h"
#include "r_internal.h"

static const size_t rope_block_size = 100;

rope_t* rope_create(const char* str) {
  size_t len;
  if (str)
    len = strlen(str);
  else
    len = 0;
  rope_t* self = __rope_create_node(len);
  if (str)
    strncpy(_str, str, len);
  _str[len] = '\0';
  _length = len;
  return self;
}

uint8_t rope_concat(rope_t* self, const char* str) {
  __rope_t* iter = _self;
  while (iter->right) {
    if (iter->right)
      iter = iter->right;
    else if (iter->left)
      iter = iter->left;
  }

  if (!iter->left)
    __rope_split_left(iter);

  __rope_attach_node(iter, rope_create(str), true);
  return EXIT_SUCCESS;
}

void rope_destroy(rope_t* self) {
  if (_left)
    rope_destroy(_left);
  if (_right)
    rope_destroy(_right);
  free(_str);
  free(_self);
}

rope_t* rope_copy(const rope_t* self) {
  rope_t* copy = rope_create(_str);
  if (_left)
    __rope_attach_node(copy, rope_copy(_left), false);
  if (_right)
    __rope_attach_node(copy, rope_copy(_right), true);
  return copy;
}

uint8_t rope_append(rope_t* self, char c) {
  __rope_t* iter = _self;
  while (iter->right) {
    if (iter->right)
      iter = iter->right;
    else if (iter->left)
      iter = iter->left;
  }

  if (iter->size > iter->length) {
    // If there's space, just put it in
    iter->str[iter->length] = c;
    iter->str[++iter->length] = '\0';
    while((iter = iter->parent))
      iter->length++;
  } else {
    // Otherwise create a new node
    if (!iter->left)
      __rope_split_left(iter);

    __rope_t* node = __rope_create_node(rope_block_size);
    node->str[node->length] = c;
    node->str[++node->length] = '\0';
    __rope_attach_node(iter, node, true);
  }
  return EXIT_SUCCESS;
}

uint8_t rope_add(rope_t* self, const rope_t* other) {
  __rope_t* iter = _self;
  while (iter->right) {
    if (iter->right)
      iter = iter->right;
    else if (iter->left)
      iter = iter->left;
  }

  if (!iter->left)
    __rope_split_left(iter);

  __rope_attach_node(iter, rope_copy(other), true);
  return EXIT_SUCCESS;
}

char* rope_str(const rope_t* self) {
  char* str = (char*)malloc(sizeof(*str) * (_length + 1));
  __rope_str(_self, str);
  return str;
}

rope_t* __rope_create_node(size_t size) {
  rope_t* self = (rope_t*)malloc(sizeof(*_self));
  _str = (char*)malloc(sizeof(*_str) * (size + 1));
  _str[0] = '\0';
  _left = NULL;
  _right = NULL;
  _parent = NULL;
  _nodes = 1;
  _size = size;
  _length = 0;
  return self;
}

void __rope_attach_node(rope_t* self, rope_t* child, bool is_right) {
  if (is_right)
    _right = child;
  else
    _left = child;

  __rope_t* iter = child;
  (((__rope_t*)child)->parent) = _self;

  while ((iter = iter->parent)) {
    iter->nodes += (((__rope_t*)child)->nodes);
    iter->length += (((__rope_t*)child)->length);
  }
}

void __rope_detach_node(rope_t* self) {
  __rope_t* iter = _self;
  if (_parent->left == _self)
    _parent->left = NULL;
  else
    _parent->right = NULL;

  while ((iter = iter->parent) != NULL) {
    iter->nodes -= _nodes;
    iter->length -= _length;
  }
  _parent = NULL;
}

char* __rope_str(const rope_t* self, char* str) {
  char* iter = str;
  int n;
  if (_left)
    iter = __rope_str(_left, iter);
  if (_right)
    iter = __rope_str(_right, iter);
  if (_str) {
    sprintf(iter, "%s%n", _str, &n);
    iter += n;
  }
  return iter;
}

void __rope_split_left(rope_t* self) {
  __rope_t temp = *_self;
  __rope_t* node = __rope_create_node(0);
  __rope_attach_node(self, node, false);
  _str = node->str;
  _size = node->size;
  node->str = temp.str;
  node->size = temp.size;
  node->length = temp.length;
}

void __rope_print(FILE* f, const rope_t* self) {
  char* str = rope_str(self);
  fprintf(f, "%s %zu %zu\n", str, _length, _nodes);
  free(str);
}
