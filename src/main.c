#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "list.h"
#include "assert.h"

uint8_t list_remove_test() {
  list_t* l = list_create(NULL, NULL, NULL);
  size_t i, len = 10;
  list_item_t* item;
  assert_notnull(ERROR, l, "List allocation failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_append(l, (list_item_t*)i),
      "List append failure."
    );
  }
  assert_false(ERROR, list_append(l, (list_item_t*)3), "List append failure.");
  assert_false(ERROR, list_remove(l, (list_item_t*)3), "List remove failure.");
  assert_false(ERROR, list_at(l, &item, 3), "List at failure.");
  list_print(l);
  assert_equal(ERROR, item, (list_item_t*)4, "List item failure.");

  assert_false(ERROR, list_destroy(l), "List destroy failure.");
  return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
  for (int i = 0; i < argc; i++) {
    printf("%s ", argv[i]);
  }
  printf("\n");

  printf("%d\n", strcmp("A", "B"));

  list_remove_test();

  return 0;
}
