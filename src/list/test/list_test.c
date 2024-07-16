#include <time.h>
#include <stdlib.h>
#include <errno.h>

#include "global.h"
#include "assert.h"
#include "list.h"

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
  list_t* l = list_create(NULL, NULL, NULL);
  assert_notnull(ERROR, l, "List allocation failure.");
  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_size_test() {
  list_t* l = list_create(NULL, NULL, NULL);
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

__attribute__((test)) uint8_t list_iter_forward_test() {
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

  assert_false(ERROR, list_head(l), "List head failure.");
  for (i = 0; i < len; i++) {
    assert_false(ERROR, list_next(l, &item), "List next failure.");
    assert_equal(ERROR, item, (list_item_t*)i, "List item failure.");
  }
  assert_true(ERROR, list_next(l, &item), "List next failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_iter_backward_test() {
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

  assert_false(ERROR, list_tail(l), "List tail failure.");
  for (i = len - 1; i < len; i--) {
    assert_false(ERROR, list_prev(l, &item), "List previous failure.");
    assert_equal(ERROR, item, (list_item_t*)i, "List item failure.");
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_iter_goto_test() {
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

__attribute__((test)) uint8_t list_iter_at_test() {
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

__attribute__((test)) uint8_t list_head_test() {
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

  assert_false(ERROR, list_head(l), "List head failure.");
  assert_false(ERROR, list_has_prev(l), "List has previous failure.");
  assert_true(ERROR, list_prev(l, &item), "List previous failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_tail_test() {
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

  assert_false(ERROR, list_tail(l), "List head failure.");
  assert_false(ERROR, list_has_next(l), "List has next failure.");
  assert_true(ERROR, list_next(l, &item), "List next failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_insert_unordered_test() {
  list_t* l = list_create(NULL, NULL, NULL);
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
      "List append failure."
    );
    assert_false(ERROR, list_at(l, &item, rnd[i]), "List goto failure.");
    assert_equal(ERROR, item, (list_item_t*)i, "List at failure.");
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_insert_ordered_test() {
  list_t* l = list_create(
    NULL,
    (int(*)(const void*, const void*))strcmp,
    NULL
  );
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

__attribute__((test)) uint8_t list_append_unordered_test() {
  list_t* l = list_create(NULL, NULL, NULL);
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
  list_t* l = list_create(
    NULL,
    (int(*)(const void*, const void*))strcmp,
    NULL
  );
  assert_notnull(ERROR, l, "List allocation failure.");

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
  list_t* l = list_create(
    NULL,
    (int(*)(const void*, const void*))strcmp,
    NULL
  );
  assert_notnull(ERROR, l, "List allocation failure.");

  assert_equal(
    ERROR,
    ENOTSUP,
    list_place(l, NULL, 0),
    "List place failure."
  );

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_place_unordered_test() {
  list_t* l = list_create(NULL, NULL, NULL);
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

  assert_equal(ERROR, len, list_size(l), "List size failure.");
  list_clear(l);
  assert_equal(ERROR, 0, list_size(l), "List size failure.");
  assert_false(ERROR, list_head(l), "List head failure.");
  assert_true(ERROR, list_next(l, &item), "List next failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_delete_test() {
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

  for (i = 0; i < list_size(l); i++) {
    assert_false(ERROR, list_delete(l, i), "List delete failure.");
    assert_false(ERROR, list_at(l, &item, i), "List at failure.");
    assert_equal(ERROR, item, (list_item_t*)(2 * i + 1), "List item failure.");
  }

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_remove_found_unordered_test() {
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
  assert_false(ERROR, list_at(l, &item, (len / 2)), "List at failure.");
  assert_equal(ERROR, item, (list_item_t*)(len / 2 + 1), "List item failure.");
  assert_false(ERROR, list_at(l, &item, list_size(l) - 1), "List at failure.");
  assert_equal(ERROR, item, (list_item_t*)(len / 2), "List item failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_remove_found_ordered_test() {
  list_t* l = list_create(
    NULL,
    (int(*)(const void*, const void*))strcmp,
    NULL
  );
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
  assert_false(ERROR, list_at(l, &item, (len / 2)), "List at failure.");
  assert_equal(
    ERROR,
    item,
    (list_item_t*)text[len / 2 + 1],
    "List item failure."
  );

  list_destroy(l);
  return EXIT_SUCCESS; // TODO
}

__attribute__((test)) uint8_t list_remove_unfound_unordered_test() {
  return EXIT_SUCCESS; // TODO
}

__attribute__((test)) uint8_t list_remove_unfound_ordered_test() {
  return EXIT_SUCCESS; // TODO
}

__attribute__((test)) uint8_t list_purge_uordered_test() {
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

  assert_false(
    ERROR,
    list_append(l, (list_item_t*)(len / 2)),
    "List append failure."
  );
  assert_false(
    ERROR,
    list_purge(l, (list_item_t*)(len / 2)),
    "List purge failure."
  );
  assert_false(ERROR, list_at(l, &item, (len / 2)), "List at failure.");
  assert_equal(ERROR, item, (list_item_t*)(len / 2 + 1), "List item failure.");
  assert_false(ERROR, list_at(l, &item, list_size(l) - 1), "List at failure.");
  assert_equal(ERROR, item, (list_item_t*)(len - 1), "List item failure.");
  assert_false(ERROR, list_delete(l, 0), "List at failure.");

  list_destroy(l);
  return EXIT_SUCCESS;
}

