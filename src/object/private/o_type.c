#include "object.h"
#include "type.h"

/// @brief Object type
const type_t* object_type = &(type_t){
  .identifier = "object",
  .destroy = object_destroy,
  .repr = object_repr,
  .hash = object_hash,
  .cmp = object_cmp
};
