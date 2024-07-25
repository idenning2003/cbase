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
#define _leaves  (_self->leaves)

typedef struct Rope __rope_t;
typedef enum Rope_Side rope_side_t;

rope_t* __rope_create_node(size_t);
void __rope_attach_node(rope_t*, rope_t*, rope_side_t);
void __rope_detach_node(rope_t*);
void __rope_move_down(rope_t*, rope_side_t);
bool __rope_is_balanced(rope_t*);
void __rope_balance(rope_t*);
rope_t** __rope_leaves_arr(rope_t*, rope_t**);
rope_t* __rope_build_from_leaves(rope_t**, size_t);
char* __rope_str(const rope_t*, char*);

enum __attribute__((packed)) Rope_Side {
  LEFT,
  RIGHT
};

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
  /// @brief Number of leaves on the left and right sub trees plus this one
  size_t leaves;
};

#endif
