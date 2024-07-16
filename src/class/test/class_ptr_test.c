#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "assert.h"
#include "class.h"

__attribute__((test)) uint8_t class_ptr_identifier_test() {
  assert_true(
    ERROR,
    !strcmp(class_identifier(ptr_class), "ptr"),
    "Pointer class identifier error."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t class_ptr_repr_test() {
  rope_t* repr;
  char* str1, str2[50];

  for (ptrdiff_t i = 1; i; i *= 2) {
    repr = class_repr(ptr_class, (void*)i);
    str1 = rope_str(repr);
    sprintf(str2, "%p", (void*)i);
    assert_true(
      ERROR,
      !strcmp(str1, str2),
      "Pointer class repr error."
    );
    free(str1);
    rope_destroy(repr);
  }

  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t class_ptr_hash_test() {
  for (ptrdiff_t i = 1; i; i *= 2) {
    assert_equal(
      ERROR,
      (uint64_t)i,
      class_hash(ptr_class, (void*)i),
      "Pointer class hash error."
    );
  }
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t class_ptr_cmp_test() {
  assert_true(
    ERROR,
    class_cmp(ptr_class, (void*)57834, (void*)4657) > 0,
    "Pointer class cmp error."
  );
  assert_true(
    ERROR,
    class_cmp(ptr_class, (void*)25646, (void*)757435) < 0,
    "Pointer class cmp error."
  );
  assert_true(
    ERROR,
    class_cmp(ptr_class, (void*)453223, (void*)453223) == 0,
    "Pointer class cmp error."
  );
  return EXIT_SUCCESS;
}
