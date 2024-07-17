#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "assert.h"
#include "type.h"

__attribute__((test)) uint8_t ptr_type_identifier_test() {
  assert_true(
    ERROR,
    !strcmp(type_identifier(ptr_type), "ptr"),
    "Pointer type identifier error."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t ptr_type_repr_test() {
  rope_t* repr;
  char* str1, str2[50];

  for (ptrdiff_t i = 1; i; i *= 2) {
    repr = type_repr(ptr_type, (void*)i);
    str1 = rope_str(repr);
    sprintf(str2, "\"%p\"", (void*)i);
    assert_true(
      ERROR,
      !strcmp(str1, str2),
      "Pointer type repr error."
    );
    free(str1);
    rope_destroy(repr);
  }

  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t ptr_type_hash_test() {
  for (ptrdiff_t i = 1; i; i *= 2) {
    assert_equal(
      ERROR,
      (uint64_t)i,
      type_hash(ptr_type, (void*)i),
      "Pointer type hash error."
    );
  }
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t ptr_type_cmp_test() {
  assert_true(
    ERROR,
    type_cmp(ptr_type, (void*)57834, (void*)4657) > 0,
    "Pointer type cmp error."
  );
  assert_true(
    ERROR,
    type_cmp(ptr_type, (void*)25646, (void*)757435) < 0,
    "Pointer type cmp error."
  );
  assert_true(
    ERROR,
    type_cmp(ptr_type, (void*)453223, (void*)453223) == 0,
    "Pointer type cmp error."
  );
  return EXIT_SUCCESS;
}
