#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "object.h"
#include "assert.h"

int main(int argc, char* argv[]) {
  for (int i = 0; i < argc; i++) {
    printf("%s ", argv[i]);
  }
  printf("\n");

  object_t* o = object_create(ptr_class, NULL, false);
  assert_notnull(ERROR, o, "Object allocation failure.");
  object_destroy(o);

  return 0;
}
