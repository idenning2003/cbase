#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "assert.h"
#include "type.h"

/// @brief NULL test type
const type_t* null_test_type = &(type_t){
  .identifier = "null test",
  .destroy = NULL,
  .copy = NULL,
  .repr = NULL,
  .hash = NULL,
  .cmp = NULL
};

__attribute__((test)) uint8_t type_null_destroy_test() {
  type_destroy(null_test_type, NULL);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t type_null_copy_test() {
  assert_equal(
    ERROR,
    (void*)0x38,
    type_copy(null_test_type, (void*)0x38),
    "NULL copy type failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t type_null_repr_test() {
  rope_t* rope = type_repr(null_test_type, (void*)0x38);
  char* str = rope_str(rope);
  assert_false(
    ERROR,
    strcmp(str, "\"0x38\""),
    "NULL repr type failure."
  );
  free(str);
  rope_destroy(rope);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t type_null_hash_test() {
  assert_equal(
    ERROR,
    0x38,
    type_hash(null_test_type, (void*)0x38),
    "NULL hash type failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t type_null_cmp_test() {
  assert_false(
    ERROR,
    type_cmp(null_test_type, (void*)0x38, (void*)0x38),
    "NULL cmp type failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t type_null_print_test() {
  size_t size = 100;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  __type_print(stream, null_test_type, (void*)0x38);
  fclose(stream);
  assert_false(
    ERROR,
    strcmp(buf, "\"0x38\"\n"),
    "NULL print type failure."
  );
  free(buf);
  return EXIT_SUCCESS;
}
