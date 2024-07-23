#ifndef R_INTERNAL_H
#define R_INTERNAL_H

#include <stddef.h>

#include "rope.h"

#define _self   ((__rope_t*)self)
#define _left   (_self->left)
#define _right  (_self->right)
#define _parent (_self->parent)
#define _str    (_self->str)
#define _length (_self->length)
#define _size   (_self->size)
#define _nodes  (_self->nodes)

typedef struct Rope __rope_t;

rope_t* __rope_create_node(size_t);
void __rope_attach_node(rope_t*, rope_t*, bool);
void __rope_detach_node(rope_t*);
void __rope_split_left(rope_t*);
char* __rope_str(const rope_t*, char*);

struct Rope {
  __rope_t* left;
  __rope_t* right;
  __rope_t* parent;
  /// @brief Only used for non-leaves
  char* str;
  /// @brief Length of all sub ropes left and right
  size_t length;
  /// @brief Size allocated for this string, zero for non-leaves
  size_t size;
  /// @brief Number of nodes on the left and right sub trees plus this one
  size_t nodes;
};

#endif
