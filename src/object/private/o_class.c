#include "object.h"
#include "class.h"

/// @brief Object class
const class_t* object_class = &(class_t){
  .identifier = "object",
  .destroy = object_destroy,
  .repr = object_repr,
  .hash = object_hash,
  .cmp = object_cmp
};
