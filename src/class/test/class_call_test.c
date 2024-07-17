#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "assert.h"
#include "class.h"

bool is_class_destroy_call_test_passed = false;
void class_destroy_call_test_func(void*) {
  is_class_destroy_call_test_passed = true;
}

bool is_class_repr_call_test_passed = false;
rope_t* class_repr_call_test_func(const void*) {
  is_class_repr_call_test_passed = true;
  return NULL;
}

bool is_class_hash_call_test_passed = false;
uint64_t class_hash_call_test_func(const void*) {
  is_class_hash_call_test_passed = true;
  return 0;
}

bool is_class_cmp_call_test_passed = false;
int class_cmp_call_test_func(const void*, const void*) {
  is_class_cmp_call_test_passed = true;
  return 0;
}

const class_t* class_func_test = &(class_t){
  .identifier = "func_test",
  .destroy = class_destroy_call_test_func,
  .repr = class_repr_call_test_func,
  .hash = class_hash_call_test_func,
  .cmp = class_cmp_call_test_func
};

__attribute__((test)) uint8_t class_call_destroy_test() {
  assert_false(
    ERROR,
    is_class_destroy_call_test_passed,
    "Not propperly set up."
  );
  class_destroy(class_func_test, NULL);
  assert_true(
    ERROR,
    is_class_destroy_call_test_passed,
    "destroy function not called."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t class_call_repr_test() {
  assert_false(
    ERROR,
    is_class_repr_call_test_passed,
    "Not propperly set up."
  );
  class_repr(class_func_test, NULL);
  assert_true(
    ERROR,
    is_class_repr_call_test_passed,
    "repr function not called."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t class_call_hash_test() {
  assert_false(
    ERROR,
    is_class_hash_call_test_passed,
    "Not propperly set up."
  );
  class_hash(class_func_test, NULL);
  assert_true(
    ERROR,
    is_class_hash_call_test_passed,
    "hash function not called."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t class_call_cmp_test() {
  assert_false(
    ERROR,
    is_class_cmp_call_test_passed,
    "Not propperly set up."
  );
  class_cmp(class_func_test, NULL, NULL);
  assert_true(
    ERROR,
    is_class_cmp_call_test_passed,
    "cmp function not called."
  );
  return EXIT_SUCCESS;
}
