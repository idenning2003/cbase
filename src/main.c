#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "global.h"
#include "assert.h"
#include "list.h"

int main(UNUSED int argc, UNUSED char* argv[]) {
  list_t* l1 = list_create(str_type, false);
  list_t* l2 = list_create(str_type, false);
  char text[][10] = {
    "First",
    "Second",
    "Third",
    "Fourth",
    "Fifth",
    "Sixth",
    "Seventh",
    "Eighth",
    "Nineth",
    "Tenth"
  };
  size_t i, len = sizeof(text) / sizeof(*text);
  assert_notnull(ERROR, l1, "List allocation failure.");
  assert_notnull(ERROR, l2, "List allocation failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_append(l1, (list_item_t*)text[i]),
      "List append failure."
    );
    assert_false(
      ERROR,
      list_append(l2, (list_item_t*)text[i]),
      "List append failure."
    );
  }

  assert_true(ERROR, list_cmp(l1, l2) == 0, "List cmp failure.");

  list_destroy(l1);
  list_destroy(l2);
  return EXIT_SUCCESS;
  return 0;
}
