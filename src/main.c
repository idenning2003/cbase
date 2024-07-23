#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "global.h"
#include "assert.h"
#include "list.h"
#include "rope.h"

int main(UNUSED int argc, UNUSED char* argv[]) {
  list_t* l = list_create(ptr_type, false);

  for (int i = 0; i < 3; i++)
    assert_false(
      ERROR,
      list_append(l, (void*)(ptrdiff_t)i),
      "List append failure."
    );

  rope_t* rope = type_repr(list_type, l);
  rope_print(rope);
  rope_destroy(rope);

  list_destroy(l);
  return 0;
}
