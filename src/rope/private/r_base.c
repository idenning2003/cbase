#include <stdlib.h>
#include <string.h>

#include "rope.h"

// TODO Implement rope instead of string.

rope_t* rope_create_with(const char* str) {
  rope_t* r = (rope_t*)malloc(sizeof(*str) * (strlen(str) + 1));
  strcpy(r, str);
  return r;
}

char* rope_str(rope_t* r) {
  return (char*)r;
}

void rope_destroy(rope_t*) {
}
