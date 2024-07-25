#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "rope.h"
#include "r_internal.h"

static const size_t rope_block_size = 5;

rope_t* rope_create(const char* str) {
  size_t len;
  if (str)
    len = strlen(str);
  else
    len = 0;
  size_t max = len;
  if (rope_block_size > max)
    max = rope_block_size;
  rope_t* self = __rope_create_node(max);
  if (str)
    strncpy(_str, str, len);
  _str[len] = '\0';
  _length = len;
  return self;
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
  __rope_t* copy = __rope_create_node(_size);
  strcpy(copy->str, _str);
  copy->length = strlen(copy->str);
  if (_left)
    __rope_attach_node(copy, rope_copy(_left), LEFT);
  if (_right)
    __rope_attach_node(copy, rope_copy(_right), RIGHT);
  return copy;
}

uint8_t rope_concat(rope_t* self, const char* str) {
  __attribute__((unused)) __rope_t* temp = _self;
  size_t len = strlen(str);
  __rope_t* iter = _self;
  while (iter->right) {
    if (iter->right)
      iter = iter->right;
    else if (iter->left)
      iter = iter->left;
  }

  if (iter->size > iter->length + len) {
    sprintf(iter->str + iter->length, "%s", str);
    iter->length += len;
    while((iter = iter->parent))
      iter->length += len;
    return EXIT_SUCCESS;
  }

  if (!iter->left)
    __rope_move_down(iter, LEFT);

  __rope_attach_node(iter, rope_create(str), RIGHT);
  return EXIT_SUCCESS;
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
      __rope_move_down(iter, LEFT);

    __rope_t* node = __rope_create_node(rope_block_size);
    node->str[node->length] = c;
    node->str[++node->length] = '\0';
    __rope_attach_node(iter, node, RIGHT);
  }
  return EXIT_SUCCESS;
}

uint8_t rope_adjoin(rope_t* self, const rope_t* other) {
  __rope_t* iter = _self;
  while (iter->right) {
    if (iter->right)
      iter = iter->right;
    else if (iter->left)
      iter = iter->left;
  }

  if (!iter->left)
    __rope_move_down(iter, LEFT);

  __rope_attach_node(iter, rope_copy(other), RIGHT);

  return EXIT_SUCCESS;
}

char* rope_str(const rope_t* self) {
  char* str = (char*)malloc(sizeof(*str) * (_length + 1));
  __rope_str(_self, str);
  return str;
}

void __rope_print(FILE* f, const rope_t* self) {
  __attribute__((unused)) __rope_t* temp = _self;
  char* str = rope_str(self);
  fprintf(f, "%s %zu %zu\n", str, _length, _leaves);
  free(str);
}

rope_t* __rope_create_node(size_t size) {
  rope_t* self = (rope_t*)malloc(sizeof(*_self));
  _str = (char*)malloc(sizeof(*_str) * (size + 1));
  _str[0] = '\0';
  _left = NULL;
  _right = NULL;
  _parent = NULL;
  _leaves = 1;
  _size = size;
  _length = 0;
  return self;
}

void __rope_attach_node(rope_t* self, rope_t* child, rope_side_t side) {
  if (side == RIGHT)
    _right = child;
  else
    _left = child;

  __rope_t* iter = child;
  (((__rope_t*)child)->parent) = _self;

  while ((iter = iter->parent)) {
    iter->leaves += (((__rope_t*)child)->leaves);
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
    iter->leaves -= _leaves;
    iter->length -= _length;
  }
  _parent = NULL;
}

void __rope_move_down(rope_t* self, rope_side_t side) {
  __rope_t temp = *_self;
  __rope_t* node = __rope_create_node(0);
  __rope_attach_node(self, node, side);
  _str = node->str;
  _size = node->size;
  node->str = temp.str;
  node->size = temp.size;
  node->length = temp.length;
}

bool __rope_is_balanced(rope_t* self) {
  return \
    (_left->leaves + 1) * 2 < _right->leaves || \
    (_right->leaves + 1) * 2 < _left->leaves;
}

void __rope_balance(rope_t* self) {
  rope_t** leaves = (rope_t**)malloc(sizeof(*leaves) * (_leaves + 1));
  leaves[0] = NULL;
  __rope_leaves_arr(self, leaves);

  size_t len;
  for (len = 0; leaves[len]; len++);
  printf("%zu\n", len);

  _leaves = 1;
  rope_t* left = __rope_build_from_leaves(leaves, len / 2 + len % 2);
  rope_t* right = __rope_build_from_leaves(leaves + len / 2 + len % 2, len / 2);
  __rope_attach_node(self, left, LEFT);
  __rope_attach_node(self, right, RIGHT);
  free(leaves);
}

rope_t** __rope_leaves_arr(rope_t* self, rope_t** leaves) {
  if (_left)
    leaves = __rope_leaves_arr(_left, leaves);
  if (_right)
    leaves = __rope_leaves_arr(_right, leaves);
  if (_size && *_str) {
    *leaves = _self;
    leaves[1] = NULL;
    leaves++;
  }
  return leaves;
}

rope_t* __rope_build_from_leaves(rope_t** leaves, size_t count) {
  if (count == 1)
    return *leaves;
  rope_t* parent = __rope_create_node(0);
  rope_t* left;
  rope_t* right;
  if (count == 2) {
    left = leaves[0];
    right = leaves[1];
  } else {
    left = __rope_build_from_leaves(leaves, count / 2 + count % 2);
    right = __rope_build_from_leaves(leaves + count / 2 + count % 2, count / 2);
  }
  __rope_attach_node(parent, left, LEFT);
  __rope_attach_node(parent, right, RIGHT);
  return parent;
}

char* __rope_str(const rope_t* self, char* str) {
  char* iter = str;
  int n;
  if (_left)
    iter = __rope_str(_left, iter);
  if (_right)
    iter = __rope_str(_right, iter);
  if (_size) {
    sprintf(iter, "%s%n", _str, &n);
    iter += n;
  }
  return iter;
}
