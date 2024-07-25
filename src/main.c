#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "global.h"
#include "assert.h"
#include "object.h"
#include "list.h"
#include "rope.h"

int main(UNUSED int argc, UNUSED char* argv[]) {
  list_t* l = list_create(object_type, true);
  object_t* o;
  assert_notnull(ERROR, l, "List allocation failure.");

  o = object_create(str_type, "Hello", false);
  assert_false(ERROR, list_append(l, (list_item_t*)o), "List append failure.");

  o = object_create(ptr_type, (void*)736, false);
  assert_false(ERROR, list_append(l, (list_item_t*)o), "List append failure.");

  list_t* sub_list = list_create(ptr_type, true);
  for (int i = 0; i < 3; i++)
    assert_false(
      ERROR,
      list_append(sub_list, (void*)(ptrdiff_t)i),
      "List append failure."
    );
  o = object_create(list_type, sub_list, false);
  assert_false(ERROR, list_append(l, (list_item_t*)o), "List append failure.");

  rope_t* rope = type_repr(list_type, l);


  __rope_balance(rope);
  rope_print(rope);

  rope_destroy(rope);
  list_destroy(l);
  list_destroy(sub_list);
  return 0;
}
