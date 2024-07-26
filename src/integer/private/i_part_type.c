#include <stdlib.h>

#include "i_internal.h"

rope_t* int_part_repr(const void* val) {
  char* str = (char*)malloc(
    sizeof(*str) *
    (sizeof(uint64_t) * 2 - __builtin_clzll(*(uint64_t*)val) / 4 + 3)
  );
  sprintf(str, "\"%lx\"", *(uint64_t*)val);
  rope_t* r = rope_create_with(str);
  free(str);
  return r;
}

uint64_t int_part_hash(const void* val) {
  return *(uint64_t*)val;
}

int int_part_cmp(const void* p1, const void* p2) {
  return *(uint64_t*)p1 - *(uint64_t*)p2;
}

const type_t* int_part_type = &(type_t){
  .identifier = "int_part",
  .destroy = free,
  .repr = int_part_repr,
  .hash = int_part_hash,
  .cmp = int_part_cmp
};
