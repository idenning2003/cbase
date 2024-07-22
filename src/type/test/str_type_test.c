#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "assert.h"
#include "type.h"

__attribute__((test)) uint8_t str_type_identifier_test() {
  assert_true(
    ERROR,
    !strcmp(type_identifier(str_type), "str"),
    "String type identifier failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t str_type_destroy_test() {
  char* str = (char*)malloc(sizeof(*str));
  type_destroy(str_type, str);
  // Rely on valgrind to verify that memory has been freed
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t str_type_repr_test() {
  rope_t* rope;
  char* dest,  str[] = "This is a test string!";

  rope = type_repr(str_type, str);
  dest = rope_str(rope);
  assert_true(
    ERROR,
    !strcmp("\"This is a test string!\"", dest),
    "String type repr failure."
  );
  free(dest);
  rope_destroy(rope);

  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t str_type_hash_test() {
  char str1[] = "This is a test string!";
  char str2[] = "This is b test string!";
  char str3[] = "This is a test string!";
  assert_equal(
    ERROR,
    type_hash(str_type, str1),
    type_hash(str_type, str3),
    "String type hash failure."
  );
  assert_notequal(
    ERROR,
    type_hash(str_type, str1),
    type_hash(str_type, str2),
    "String type hash failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t str_type_cmp_test() {
  char str1[] = "This is a test string!";
  char str2[] = "This is b test string!";
  char str3[] = "This is a test string!";
  assert_true(
    ERROR,
    type_cmp(str_type, str2, str1) > 0,
    "String type cmp failure."
  );
  assert_true(
    ERROR,
    type_cmp(str_type, str3, str2) < 0,
    "String type cmp failure."
  );
  assert_true(
    ERROR,
    type_cmp(str_type, str1, str3) == 0,
    "String type cmp failure."
  );
  return EXIT_SUCCESS;
}
