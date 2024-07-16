#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "assert.h"
#include "class.h"

__attribute__((test)) uint8_t class_str_name_test() {
  assert_true(
    ERROR,
    !strcmp(class_name(str_class), "str"),
    "String class name error."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t class_str_destroy_test() {
  char* str = (char*)malloc(sizeof(*str));
  class_destroy(str_class, str);
  // Rely on valgrind to verify that memory has been freed
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t class_str_repr_test() {
  rope_t* repr;
  char* dest,  str[] = "This is a test string!";

  repr = class_repr(str_class, str);
  dest = rope_str(repr);
  assert_true(
    ERROR,
    !strcmp(str, dest),
    "String class repr error."
  );
  free(dest);
  rope_destroy(repr);

  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t class_str_hash_test() {
  char str1[] = "This is a test string!";
  char str2[] = "This is b test string!";
  char str3[] = "This is a test string!";
  assert_equal(
    ERROR,
    class_hash(str_class, str1),
    class_hash(str_class, str3),
    "String class hash error."
  );
  assert_notequal(
    ERROR,
    class_hash(str_class, str1),
    class_hash(str_class, str2),
    "String class hash error."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t class_str_cmp_test() {
  char str1[] = "This is a test string!";
  char str2[] = "This is b test string!";
  char str3[] = "This is a test string!";
  assert_true(
    ERROR,
    class_cmp(str_class, str2, str1) > 0,
    "String class cmp error."
  );
  assert_true(
    ERROR,
    class_cmp(str_class, str3, str2) < 0,
    "String class cmp error."
  );
  assert_true(
    ERROR,
    class_cmp(str_class, str1, str3) == 0,
    "String class cmp error."
  );
  return EXIT_SUCCESS;
}
