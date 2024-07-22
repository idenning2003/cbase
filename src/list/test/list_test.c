#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "global.h"
#include "assert.h"
#include "list.h"
#include "object.h"

void rand_int_arr(int* arr, size_t len) {
  size_t i;
  bool flag[len];
  for (i = 0; i < len; i++)
    flag[i] = false;
  for (i = 0; i < len; i++) {
    int v = rand() % (len - i) + 1;
    for (size_t j = 0; j < len; j++) {
      if (!flag[j] && !--v) {
        flag[j] = true;
        arr[i] = j;
        break;
      }
    }
  }
}

__attribute__((test)) uint8_t list_create_destroy_test() {
  list_t* l = list_create(ptr_type, true);
  assert_notnull(ERROR, l, "List allocation failure.");
  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_mixed_type_test() {
  list_t* l = list_create(object_type, true);
  object_t* o;
  char* solution =  "[\"Hello\", \"0x2e0\", [\"0\", \"0x1\", \"0x2\"]]";
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
  char* str = rope_str(rope);
  assert_false(ERROR, strcmp(solution, str), "List multitype failure.");
  free(rope);
  free(str);

  list_destroy(l);
  list_destroy(sub_list);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_size_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i;
  assert_notnull(ERROR, l, "List allocation failure.");
  for (i = 0; i < 10; i++) {
    assert_equal(ERROR, i, list_size(l), "Size appending failure.");
    assert_false(
      ERROR,
      list_append(l, (list_item_t*)i),
      "List append failure."
    );
  }
  for (; i > 5; i--) {
    assert_equal(ERROR, i, list_size(l), "Size deleting failure.");
    assert_false(ERROR, list_delete(l, 0), "List delete failure.");
  }
  assert_false(ERROR, list_clear(l), "List clear failure.");
  assert_equal(ERROR, 0, list_size(l), "Size clear failure.");
  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_next_test() {
  list_t* l = list_create(ptr_type, true);
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

  assert_false(ERROR, list_head(l), "List head failure.");
  for (i = 0; i < len; i++) {
    assert_false(ERROR, list_next(l, &item), "List next failure.");
    assert_equal(ERROR, item, (list_item_t*)i, "List item failure.");
  }
  assert_true(ERROR, list_next(l, &item), "List next failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_prev_test() {
  list_t* l = list_create(ptr_type, true);
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

  assert_false(ERROR, list_tail(l), "List tail failure.");
  for (i = len - 1; i < len; i--) {
    assert_false(ERROR, list_prev(l, &item), "List previous failure.");
    assert_equal(ERROR, item, (list_item_t*)i, "List item failure.");
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_has_next_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 10;
  assert_notnull(ERROR, l, "List allocation failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_append(l, (list_item_t*)i),
      "List append failure."
    );
  }

  assert_false(ERROR, list_tail(l), "List tail failure.");
  assert_false(ERROR, list_has_next(l), "List has next failure.");
  assert_false(ERROR, list_prev(l, NULL), "List prev failure.");
  assert_false(ERROR, list_has_next(l), "List has next failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_has_prev_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 10;
  assert_notnull(ERROR, l, "List allocation failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_append(l, (list_item_t*)i),
      "List append failure."
    );
  }

  assert_false(ERROR, list_head(l), "List head failure.");
  assert_false(ERROR, list_has_prev(l), "List has prev failure.");
  assert_false(ERROR, list_next(l, NULL), "List next failure.");
  assert_false(ERROR, list_has_prev(l), "List has prev failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_goto_test() {
  list_t* l = list_create(ptr_type, true);
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

  srand(time(NULL));
  size_t rnd[len - 1];
  bool flag[len - 1];
  for (i = 0; i < len - 1; i++)
    flag[i] = false;
  for (i = 0; i < len - 1; i++) {
    int v = rand() % (len - i - 1) + 1;
    for (size_t j = 0; j < len - 1; j++) {
      if (!flag[j] && !--v) {
        flag[j] = true;
        rnd[i] = j;
        break;
      }
    }
  }

  for (i = 0; i < sizeof(rnd)/sizeof(*rnd); i++) {
    assert_false(ERROR, list_goto(l, rnd[i]), "List goto failure.");
    assert_false(ERROR, list_next(l, &item), "List next failure.");
    assert_equal(ERROR, item, (list_item_t*)rnd[i] + 1, "List item failure.");
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_at_test1() {
  list_t* l = list_create(ptr_type, true);
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

  srand(time(NULL));
  int rnd[len - 1];
  rand_int_arr(rnd, len - 1);

  for (i = 0; i < sizeof(rnd)/sizeof(*rnd); i++) {
    assert_false(ERROR, list_at(l, &item, rnd[i]), "List at failure.");
    assert_equal(
      ERROR,
      item,
      (list_item_t*)(ptrdiff_t)rnd[i],
      "List item failure."
    );
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_at_test2() {
  list_t* l = list_create(ptr_type, true);
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

  srand(time(NULL));
  int rnd[len - 1];
  rand_int_arr(rnd, len - 1);

  assert_equal(ERROR, ERANGE, list_at(l, &item, len + 1), "List at failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_head_test() {
  list_t* l = list_create(ptr_type, true);
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

  assert_false(ERROR, list_head(l), "List head failure.");
  assert_false(ERROR, list_has_prev(l), "List has previous failure.");
  assert_true(ERROR, list_prev(l, &item), "List previous failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_tail_test() {
  list_t* l = list_create(ptr_type, true);
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

  assert_false(ERROR, list_tail(l), "List head failure.");
  assert_false(ERROR, list_has_next(l), "List has next failure.");
  assert_true(ERROR, list_next(l, &item), "List next failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_insert_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 10;
  list_item_t* item;

  srand(time(NULL));
  int rnd[len];
  rand_int_arr(rnd, len);

  for (i = 0; i < sizeof(rnd)/sizeof(*rnd); i++) {
    assert_false(
      ERROR,
      list_append(l, (list_item_t*)i),
      "List append failure."
    );
  }

  for (i = 0; i < sizeof(rnd)/sizeof(*rnd); i++) {
    assert_false(ERROR, list_goto(l, rnd[i]), "List goto failure.");
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)i),
      "List insert failure."
    );
    assert_false(ERROR, list_at(l, &item, rnd[i]), "List at failure.");
    assert_equal(ERROR, item, (list_item_t*)i, "List at item failure.");
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_insert_ordered_test() {
  list_t* l = list_create(str_type, false);
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
  list_item_t* item, *last = NULL;
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)text[i]),
      "List insert failure."
    );
  }

  assert_false(ERROR, list_head(l), "List head failure.");
  for (i = 0; i < len; i++) {
    assert_false(ERROR, list_next(l, &item), "List next failure.");
    if (last) {
      assert_true(ERROR, strcmp(item, last) >= 0, "List order failure.");
    }
    last = item;
  }
  assert_true(ERROR, list_next(l, &item), "List next failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_insert_reverse_ordered_test() {
  list_t* l = list_create(str_type, false);
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
  list_item_t* item, *last = NULL;
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");
  assert_false(ERROR, list_reverse(l), "List reverse failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)text[i]),
      "List insert failure."
    );
  }

  assert_false(ERROR, list_head(l), "List head failure.");
  for (i = 0; i < len; i++) {
    assert_false(ERROR, list_next(l, &item), "List next failure.");
    if (last) {
      assert_true(ERROR, strcmp(item, last) <= 0, "List order failure.");
    }
    last = item;
  }
  assert_true(ERROR, list_next(l, &item), "List next failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_insert_head_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 10;

  for (i = 0; i < len; i++) {
    list_head(l);
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)i),
      "List insert failure."
    );
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_append_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, j, len = 10;
  list_item_t* item;
  assert_notnull(ERROR, l, "List allocation failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_append(l, (list_item_t*)i),
      "List append failure."
    );
    assert_false(ERROR, list_head(l), "List head failure.");
    for (j = 0; j <= i; j++) {
      assert_false(ERROR, list_next(l, &item), "List next failure.");
      assert_equal(ERROR, item, (list_item_t*)j, "List item failure.");
    }
    assert_true(ERROR, list_next(l, &item), "List next failure.");
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_append_ordered_test() {
  list_t* l = list_create(str_type, true);
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");

  assert_equal(
    ERROR,
    ENOTSUP,
    list_append(l, NULL),
    "List append failure."
  );

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_place_ordered_test() {
  list_t* l = list_create(str_type, true);
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");

  assert_equal(
    ERROR,
    ENOTSUP,
    list_place(l, NULL, 0),
    "List place failure."
  );

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_place_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 10;
  list_item_t* item;
  assert_notnull(ERROR, l, "List allocation failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_place(l, (list_item_t*)i, list_size(l)),
      "List place failure."
    );
  }

  srand(time(NULL));
  int rnd[len];
  rand_int_arr(rnd, len);

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_place(l, (list_item_t*)i, rnd[i]),
      "List place failure."
    );
    assert_false(ERROR, list_at(l, &item, rnd[i]), "List at failure.");
    assert_equal(ERROR, item, (list_item_t*)i, "List item failure.");
  }
  assert_true(
      ERROR,
      list_place(l, (list_item_t*)i, list_size(l) + 1),
      "List place failure."
    );

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_clear_test() {
  list_t* l = list_create(ptr_type, true);
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

  assert_equal(ERROR, len, list_size(l), "List size failure.");
  list_clear(l);
  assert_equal(ERROR, 0, list_size(l), "List size failure.");
  assert_false(ERROR, list_head(l), "List head failure.");
  assert_true(ERROR, list_next(l, &item), "List next failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_delete_test() {
  list_t* l = list_create(ptr_type, true);
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

  for (i = 0; i < list_size(l); i++) {
    assert_false(ERROR, list_delete(l, i), "List delete failure.");
    assert_false(ERROR, list_at(l, &item, i), "List at failure.");
    assert_equal(ERROR, item, (list_item_t*)(2 * i + 1), "List item failure.");
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_remove_test() {
  list_t* l = list_create(ptr_type, true);
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
  assert_false(
    ERROR,
    list_append(l, (list_item_t*)(len / 2)),
    "List append failure."
  );

  assert_false(
    ERROR,
    list_remove(l, (list_item_t*)(len / 2)),
    "List remove failure."
  );

  list_head(l);
  for (i = 0; i < len - 1; i++) {
    assert_false(ERROR, list_next(l, &item), "List next failure.");
    assert_notequal(ERROR, item, (list_item_t*)(len / 2), "List item failure.");
  }
  assert_false(ERROR, list_next(l, &item), "List next failure.");
  assert_equal(ERROR, item, (list_item_t*)(len / 2), "List item failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_remove_ordered_test() {
  list_t* l = list_create(str_type, false);
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
  list_item_t* item = NULL;
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)text[i]),
      "List insert failure."
    );
  }

  assert_false(
    ERROR,
    list_remove(l, (list_item_t*)"First"),
    "List remove failure."
  );

  list_head(l);
  while (!list_next(l, &item)) {
    if (!strcmp("First", item))
      assert_fail(ERROR, "List purge item failure.");
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_remove_reverse_ordered_test() {
  list_t* l = list_create(str_type, false);
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
  list_item_t* item = NULL;
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");
  assert_false(ERROR, list_reverse(l), "List reverse failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)text[i]),
      "List insert failure."
    );
  }

  assert_false(
    ERROR,
    list_remove(l, (list_item_t*)"First"),
    "List remove failure."
  );

  list_head(l);
  while (!list_next(l, &item)) {
    if (!strcmp("First", item))
      assert_fail(ERROR, "List purge item failure.");
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_remove_unfound_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 10;
  assert_notnull(ERROR, l, "List allocation failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_append(l, (list_item_t*)i),
      "List append failure."
    );
  }

  assert_equal(
    ERROR,
    ENOENT,
    list_remove(l, (list_item_t*)(len + 1)),
    "List remove no entry failure."
  );

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_remove_unfound_ordered_test() {
  list_t* l = list_create(str_type, false);
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
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)text[i]),
      "List insert failure."
    );
  }

  assert_equal(
    ERROR,
    ENOENT,
    list_remove(l, "Eleventh"),
    "List remove no entry failure."
  );

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_remove_unfound_reverse_ordered_test() {
  list_t* l = list_create(str_type, false);
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
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");
  assert_false(ERROR, list_reverse(l), "List reverse failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)text[i]),
      "List insert failure."
    );
  }

  assert_equal(
    ERROR,
    ENOENT,
    list_remove(l, "Eleventh"),
    "List remove no entry failure."
  );

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_purge_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 10, purge_item = 5;
  list_item_t* item;
  assert_notnull(ERROR, l, "List allocation failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_append(l, (list_item_t*)i),
      "List append failure."
    );
  }

  assert_false(
    ERROR,
    list_append(l, (list_item_t*)purge_item),
    "List append failure."
  );
  assert_false(
    ERROR,
    list_purge(l, (list_item_t*)purge_item),
    "List purge failure."
  );

  list_head(l);
  while (!list_next(l, &item)) {
    if (item == (list_item_t*)purge_item)
      assert_fail(ERROR, "List purge item failure.");
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_purge_ordered_test1() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 10, purge_item = 5;
  list_item_t* item;
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)i),
      "List insert failure."
    );
  }

  assert_false(
    ERROR,
    list_insert(l, (list_item_t*)purge_item),
    "List insert failure."
  );
  assert_false(
    ERROR,
    list_purge(l, (list_item_t*)purge_item),
    "List purge failure."
  );

  list_head(l);
  while (!list_next(l, &item)) {
    if (item == (list_item_t*)purge_item)
      assert_fail(ERROR, "List purge item failure.");
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_purge_ordered_test2() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 10, purge_item = 0;
  list_item_t* item;
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)i),
      "List insert failure."
    );
  }

  assert_false(
    ERROR,
    list_insert(l, (list_item_t*)purge_item),
    "List insert failure."
  );
  assert_false(
    ERROR,
    list_purge(l, (list_item_t*)purge_item),
    "List purge failure."
  );

  list_head(l);
  while (!list_next(l, &item)) {
    if (item == (list_item_t*)purge_item)
      assert_fail(ERROR, "List purge item failure.");
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_purge_reverse_ordered_test1() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 10, purge_item = 5;
  list_item_t* item;
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");
  assert_false(ERROR, list_reverse(l), "List reverse failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)i),
      "List insert failure."
    );
  }

  assert_false(
    ERROR,
    list_insert(l, (list_item_t*)purge_item),
    "List insert failure."
  );
  assert_false(
    ERROR,
    list_purge(l, (list_item_t*)purge_item),
    "List purge failure."
  );

  list_head(l);
  while (!list_next(l, &item)) {
    if (item == (list_item_t*)purge_item)
      assert_fail(ERROR, "List purge item failure.");
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_purge_reverse_ordered_test2() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 10, purge_item = 0;
  list_item_t* item;
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");
  assert_false(ERROR, list_reverse(l), "List reverse failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)i),
      "List insert failure."
    );
  }

  assert_false(
    ERROR,
    list_insert(l, (list_item_t*)purge_item),
    "List insert failure."
  );
  assert_false(
    ERROR,
    list_purge(l, (list_item_t*)purge_item),
    "List purge failure."
  );

  list_head(l);
  while (!list_next(l, &item)) {
    if (item == (list_item_t*)purge_item)
      assert_fail(ERROR, "List purge item failure.");
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_purge_unfound_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 10;
  assert_notnull(ERROR, l, "List allocation failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_append(l, (list_item_t*)i),
      "List append failure."
    );
  }

  assert_equal(
    ERROR,
    ENOENT,
    list_purge(l, (list_item_t*)len),
    "List purge failure."
  );

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_purge_unfound_ordered_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 10;
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)i),
      "List insert failure."
    );
  }

  assert_equal(
    ERROR,
    ENOENT,
    list_purge(l, (list_item_t*)len),
    "List purge failure."
  );

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_copy_unorderd_test() {
  list_t* l = list_create(ptr_type, true);
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

  list_t* other = list_copy(l);
  list_destroy(l);

  assert_false(ERROR, list_head(other), "List head failure.");
  for (i = 0; i < len; i++) {
    assert_false(ERROR, list_next(other, &item), "List next failure.");
    assert_equal(ERROR, item, (list_item_t*)i, "List item failure.");
  }
  assert_true(ERROR, list_next(other, &item), "List next failure.");

  list_destroy(other);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_copy_orderd_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 10;
  list_item_t* item;
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)i),
      "List insert failure."
    );
  }

  list_t* other = list_copy(l);
  list_destroy(l);

  assert_false(ERROR, list_head(other), "List head failure.");
  for (i = 0; i < len; i++) {
    assert_false(ERROR, list_next(other, &item), "List next failure.");
    assert_equal(ERROR, item, (list_item_t*)i, "List item failure.");
  }
  assert_true(ERROR, list_next(other, &item), "List next failure.");

  list_destroy(other);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_copy_reverse_orderd_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 10;
  list_item_t* item;
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");
  assert_false(ERROR, list_reverse(l), "List reverse failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)i),
      "List insert failure."
    );
  }

  list_t* other = list_copy(l);
  list_destroy(l);

  assert_false(ERROR, list_head(other), "List head failure.");
  for (i = len - 1; i > 0; i--) {
    assert_false(ERROR, list_next(other, &item), "List next failure.");
    assert_equal(ERROR, item, (list_item_t*)i, "List item failure.");
  }
  assert_false(ERROR, list_next(other, &item), "List next failure.");
  assert_equal(ERROR, item, (list_item_t*)i, "List item failure.");
  assert_true(ERROR, list_next(other, &item), "List next failure.");

  list_destroy(other);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_reverse_even_size_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 10;
  list_item_t* item, *last = NULL;
  assert_notnull(ERROR, l, "List allocation failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_append(l, (list_item_t*)i),
      "List append failure."
    );
  }

  assert_false(ERROR, list_size(l) % 2, "List size failure.");
  assert_false(ERROR, list_reverse(l), "List reverse failure.");

  assert_false(ERROR, list_head(l), "List head failure.");
  for (i = 0; i < len; i++) {
    assert_false(ERROR, list_next(l, &item), "List next failure.");
    if (last) {
      assert_true(ERROR, item < last, "List order failure.");
    }
    last = item;
  }
  assert_true(ERROR, list_next(l, &item), "List next failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_reverse_odd_size_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 9;
  list_item_t* item, *last = NULL;
  assert_notnull(ERROR, l, "List allocation failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_append(l, (list_item_t*)i),
      "List append failure."
    );
  }

  assert_true(ERROR, list_size(l) % 2, "List size failure.");
  assert_false(ERROR, list_reverse(l), "List reverse failure.");

  assert_false(ERROR, list_head(l), "List head failure.");
  for (i = 0; i < len; i++) {
    assert_false(ERROR, list_next(l, &item), "List next failure.");
    if (last) {
      assert_true(ERROR, item < last, "List order failure.");
    }
    last = item;
  }
  assert_true(ERROR, list_next(l, &item), "List next failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_order_test() {
  list_t* l = list_create(str_type, false);
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
  list_item_t* item, *last = NULL;
  assert_notnull(ERROR, l, "List allocation failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_append(l, (list_item_t*)text[i]),
      "List append failure."
    );
  }
  list_order(l);

  assert_false(ERROR, list_head(l), "List head failure.");
  for (i = 0; i < len; i++) {
    assert_false(ERROR, list_next(l, &item), "List next failure.");
    if (last) {
      assert_true(ERROR, strcmp(item, last) >= 0, "List order failure.");
    }
    last = item;
  }
  assert_true(ERROR, list_next(l, &item), "List next failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_unorder_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 10;
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");

  for (i = len / 2; i < len; i++) {
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)i),
      "List insert failure."
    );
  }

  list_unorder(l);

  for (i = 0; i < len / 2; i++) {
    assert_false(
      ERROR,
      list_append(l, (list_item_t*)i),
      "List append failure."
    );
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_indexof_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, index, len = 10;
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)i),
      "List insert failure."
    );
  }

  srand(time(NULL));
  int rnd[len - 1];
  rand_int_arr(rnd, len - 1);

  for (i = 0; i < sizeof(rnd)/sizeof(*rnd); i++) {
    assert_false(
      ERROR,
      list_indexof(l, &index, (list_item_t*)(ptrdiff_t)rnd[i]),
      "List indexof failure."
    );
    assert_equal(ERROR, index, (size_t)rnd[i], "List indexof value failure.");
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_indexof_ordered_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, index, len = 10;
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)i),
      "List insert failure."
    );
  }

  srand(time(NULL));
  int rnd[len - 1];
  rand_int_arr(rnd, len - 1);

  for (i = 0; i < sizeof(rnd)/sizeof(*rnd); i++) {
    assert_false(
      ERROR,
      list_indexof(l, &index, (list_item_t*)(ptrdiff_t)rnd[i]),
      "List indexof failure."
    );
    assert_equal(ERROR, index, (size_t)rnd[i], "List indexof value failure.");
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_indexof_reverse_ordered_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, index, len = 10;
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");
  assert_false(ERROR, list_reverse(l), "List reverse failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)i),
      "List insert failure."
    );
  }

  srand(time(NULL));
  int rnd[len - 1];
  rand_int_arr(rnd, len - 1);

  for (i = 0; i < sizeof(rnd)/sizeof(*rnd); i++) {
    assert_false(
      ERROR,
      list_indexof(l, &index, (list_item_t*)(ptrdiff_t)rnd[i]),
      "List indexof failure."
    );
    assert_equal(
      ERROR,
      index,
      (size_t)(-rnd[i] + len - 1),
      "List indexof value failure."
    );
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_indexof_unfound_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, index, len = 10;
  assert_notnull(ERROR, l, "List allocation failure.");

  for (i = 0; i < len; i++) {
    if (i == len / 2)
      continue;
    assert_false(
      ERROR,
      list_append(l, (list_item_t*)i),
      "List append failure."
    );
  }

  assert_equal(
    ERROR,
    ENOENT,
    list_indexof(l, &index, (void*)(len / 2)),
    "List indexof failure."
  );

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_indexof_unfound_ordered_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, index, len = 10;
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");

  for (i = 0; i < len; i++) {
    if (i == len / 2)
      continue;
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)i),
      "List insert failure."
    );
  }

  assert_equal(
    ERROR,
    ENOENT,
    list_indexof(l, &index, (void*)(len / 2)),
    "List indexof failure."
  );

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_indexof_unfound_reverse_ordered_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, index, len = 10;
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");
  assert_false(ERROR, list_reverse(l), "List reverse failure.");

  for (i = 0; i < len; i++) {
    if (i == len / 2)
      continue;
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)i),
      "List insert failure."
    );
  }

  assert_equal(
    ERROR,
    ENOENT,
    list_indexof(l, &index, (void*)(len / 2)),
    "List indexof failure."
  );

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_indexof_empty_test() {
  list_t* l = list_create(ptr_type, true);
  size_t index;
  assert_notnull(ERROR, l, "List allocation failure.");


  assert_equal(
    ERROR,
    ENOENT,
    list_indexof(l, &index, (void*)0),
    "List indexof failure."
  );

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_contains_test() {
  list_t* l = list_create(ptr_type, true);
  size_t i, len = 10;
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_false(ERROR, list_order(l), "List order failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_insert(l, (list_item_t*)i),
      "List insert failure."
    );
  }

  srand(time(NULL));
  int rnd[len - 1];
  rand_int_arr(rnd, len - 1);

  for (i = 0; i < sizeof(rnd)/sizeof(*rnd); i++) {
    assert_true(
      ERROR,
      list_contains(l, (list_item_t*)(ptrdiff_t)rnd[i]),
      "List contains failure."
    );
  }

  assert_false(
    ERROR,
    list_contains(l, (list_item_t*)(ptrdiff_t)len),
    "List contains failure."
  );

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_cmp_test1() {
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
}

__attribute__((test)) uint8_t list_cmp_test2() {
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
  assert_false(
    ERROR,
    list_append(l1, (list_item_t*)"Eleventh"),
    "List append failure."
  );

  assert_true(ERROR, list_cmp(l1, l2) > 0, "List cmp failure.");

  list_destroy(l1);
  list_destroy(l2);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_cmp_test3() {
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
  assert_false(
    ERROR,
    list_append(l2, (list_item_t*)"Eleventh"),
    "List append failure."
  );

  assert_true(ERROR, list_cmp(l1, l2) < 0, "List cmp failure.");

  list_destroy(l1);
  list_destroy(l2);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_cmp_test4() {
  list_t* l1 = list_create(str_type, false);
  list_t* l2 = list_create(str_type, false);
  char text1[][10] = {
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
  char text2[][10] = {
    "First",
    "Second",
    "Third",
    "Fourth",
    "Fifth",
    "Sixth",
    "Seventh",
    "Eighth",
    "Nineth",
    "Tentg"
  };
  size_t i, len = sizeof(text1) / sizeof(*text1);
  assert_notnull(ERROR, l1, "List allocation failure.");
  assert_notnull(ERROR, l2, "List allocation failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_append(l1, (list_item_t*)text1[i]),
      "List append failure."
    );
    assert_false(
      ERROR,
      list_append(l2, (list_item_t*)text2[i]),
      "List append failure."
    );
  }

  assert_true(ERROR, list_cmp(l1, l2) > 0, "List cmp failure.");

  list_destroy(l1);
  list_destroy(l2);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_cmp_test5() {
  list_t* l1 = list_create(str_type, false);
  list_t* l2 = list_create(str_type, false);
  char text1[][10] = {
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
  char text2[][10] = {
    "First",
    "Second",
    "Third",
    "Fourth",
    "Fifth",
    "Sixth",
    "Seventh",
    "Eighth",
    "Nineth",
    "Tenti"
  };
  size_t i, len = sizeof(text1) / sizeof(*text1);
  assert_notnull(ERROR, l1, "List allocation failure.");
  assert_notnull(ERROR, l2, "List allocation failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_append(l1, (list_item_t*)text1[i]),
      "List append failure."
    );
    assert_false(
      ERROR,
      list_append(l2, (list_item_t*)text2[i]),
      "List append failure."
    );
  }

  assert_true(ERROR, list_cmp(l1, l2) < 0, "List cmp failure.");

  list_destroy(l1);
  list_destroy(l2);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_cmp_test6() {
  list_t* l1 = list_create(str_type, false);
  list_t* l2 = list_create(ptr_type, false);
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
      list_append(l2, NULL),
      "List append failure."
    );
  }

  assert_true(ERROR, list_cmp(l1, l2) != 0, "List cmp failure.");

  list_destroy(l1);
  list_destroy(l2);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_typeof_test1() {
  list_t* l = list_create(ptr_type, true);
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_equal(ERROR, ptr_type, list_typeof(l), "List typeof failure.");
  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_typeof_test2() {
  list_t* l = list_create(NULL, true);
  assert_notnull(ERROR, l, "List allocation failure.");
  assert_equal(ERROR, ptr_type, list_typeof(l), "List typeof failure.");
  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_print_test() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  char* solution = "[\"First\", \"Second\", \"Third\"]\n";
  list_t* l = list_create(str_type, false);
  char text[][10] = {
    "First",
    "Second",
    "Third"
  };
  size_t i, len = sizeof(text) / sizeof(*text);
  assert_notnull(ERROR, l, "List allocation failure.");

  for (i = 0; i < len; i++) {
    assert_false(
      ERROR,
      list_append(l, (list_item_t*)text[i]),
      "List append failure."
    );
  }

  __list_print(stream, l);
  fclose(stream);

  assert_false(ERROR, strcmp(solution, buf), "List print failure.");

  free(buf);
  list_destroy(l);
  return EXIT_SUCCESS;
}
