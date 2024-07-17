#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "assert.h"
#include "type.h"

bool is_type_destroy_call_test_passed = false;
void type_destroy_call_test_func(void*) {
  is_type_destroy_call_test_passed = true;
}

bool is_type_repr_call_test_passed = false;
rope_t* type_repr_call_test_func(const void*) {
  is_type_repr_call_test_passed = true;
  return NULL;
}

bool is_type_hash_call_test_passed = false;
uint64_t type_hash_call_test_func(const void*) {
  is_type_hash_call_test_passed = true;
  return 0;
}

bool is_type_cmp_call_test_passed = false;
int type_cmp_call_test_func(const void*, const void*) {
  is_type_cmp_call_test_passed = true;
  return 0;
}

const type_t* type_func_test = &(type_t){
  .identifier = "func_test",
  .destroy = type_destroy_call_test_func,
  .repr = type_repr_call_test_func,
  .hash = type_hash_call_test_func,
  .cmp = type_cmp_call_test_func
};

__attribute__((test)) uint8_t type_call_destroy_test() {
  assert_false(
    ERROR,
    is_type_destroy_call_test_passed,
    "Not propperly set up."
  );
  type_destroy(type_func_test, NULL);
  assert_true(
    ERROR,
    is_type_destroy_call_test_passed,
    "destroy function not called."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t type_call_repr_test() {
  assert_false(
    ERROR,
    is_type_repr_call_test_passed,
    "Not propperly set up."
  );
  type_repr(type_func_test, NULL);
  assert_true(
    ERROR,
    is_type_repr_call_test_passed,
    "repr function not called."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t type_call_hash_test() {
  assert_false(
    ERROR,
    is_type_hash_call_test_passed,
    "Not propperly set up."
  );
  type_hash(type_func_test, NULL);
  assert_true(
    ERROR,
    is_type_hash_call_test_passed,
    "hash function not called."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t type_call_cmp_test() {
  assert_false(
    ERROR,
    is_type_cmp_call_test_passed,
    "Not propperly set up."
  );
  type_cmp(type_func_test, NULL, NULL);
  assert_true(
    ERROR,
    is_type_cmp_call_test_passed,
    "cmp function not called."
  );
  return EXIT_SUCCESS;
}
