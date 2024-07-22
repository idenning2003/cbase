#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "global.h"
#include "assert.h"
#include "list.h"

__attribute__((test)) uint8_t list_type_identifier_test() {
  assert_true(
    ERROR,
    !strcmp(type_identifier(list_type), "list"),
    "String type identifier error."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_type_destroy_test() {
  list_t* l = list_create(ptr_type, false);
  type_destroy(list_type, l);
  // Rely on valgrind to verify that memory has been freed
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_type_repr_test() {
  list_t* l = list_create(ptr_type, false);
  char* solution = "[\"0\", \"0x1\", \"0x2\"]";

  for (int i = 0; i < 3; i++)
    assert_false(
      ERROR,
      list_append(l, (void*)(ptrdiff_t)i),
      "List append failure."
    );

  rope_t* rope = type_repr(list_type, l);
  char* str = rope_str(rope);
  assert_true(
    ERROR,
    !strcmp(solution, str),
    "List type repr failure."
  );
  free(str);
  rope_destroy(rope);

  list_destroy(l);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_type_hash_test() {
  list_t* l1 = list_create(ptr_type, false);
  list_t* l2 = list_create(ptr_type, false);
  list_t* l3 = list_create(ptr_type, false);

  for (int i = 0; i < 3; i++) {
    assert_false(
      ERROR,
      list_append(l1, (void*)(ptrdiff_t)i),
      "List append failure."
    );
    assert_false(
      ERROR,
      list_append(l2, (void*)(ptrdiff_t)(i + 1)),
      "List append failure."
    );
    assert_false(
      ERROR,
      list_append(l3, (void*)(ptrdiff_t)i),
      "List append failure."
    );
  }

  assert_notequal(
    ERROR,
    type_hash(list_type, l1),
    type_hash(list_type, l2),
    "String type hash failure."
  );
  assert_equal(
    ERROR,
    type_hash(list_type, l1),
    type_hash(list_type, l3),
    "String type hash failure."
  );

  list_destroy(l1);
  list_destroy(l2);
  list_destroy(l3);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t list_type_cmp_test() {
  list_t* l1 = list_create(ptr_type, false);
  list_t* l2 = list_create(ptr_type, false);
  list_t* l3 = list_create(ptr_type, false);

  for (int i = 0; i < 3; i++) {
    assert_false(
      ERROR,
      list_append(l1, (void*)(ptrdiff_t)i),
      "List append failure."
    );
    assert_false(
      ERROR,
      list_append(l2, (void*)(ptrdiff_t)(i + 1)),
      "List append failure."
    );
    assert_false(
      ERROR,
      list_append(l3, (void*)(ptrdiff_t)i),
      "List append failure."
    );
  }

  assert_true(
    ERROR,
    type_cmp(list_type, l1, l2) < 0,
    "String type cmp failure."
  );
  assert_true(
    ERROR,
    type_cmp(list_type, l2, l3) > 0,
    "String type cmp failure."
  );
  assert_true(
    ERROR,
    type_cmp(list_type, l3, l1) == 0,
    "String type cmp failure."
  );

  list_destroy(l1);
  list_destroy(l2);
  list_destroy(l3);
  return EXIT_SUCCESS;
}
