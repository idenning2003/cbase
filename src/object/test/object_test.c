#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "assert.h"
#include "object.h"

__attribute__((test)) uint8_t object_create_test() {
  object_t* o = object_create(ptr_type, NULL, true);
  assert_notnull(ERROR, o, "Object allocation failure.");
  object_destroy(o);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t object_destroy_test1() {
  char* str = (char*)malloc(sizeof(*str) * 10);
  strcpy(str, "Test");
  object_t* o = object_create(str_type, str, true);
  assert_notnull(ERROR, o, "Object allocation failure.");
  object_destroy(o);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t object_detatch_test2() {
  object_t* o = object_create(str_type, NULL, false);
  assert_notnull(ERROR, o, "Object allocation failure.");
  object_destroy(o);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t object_copy_test1() {
  char str[] = "Test";
  object_t* o = object_create(str_type, str, false);
  assert_notnull(ERROR, o, "Object allocation failure.");
  object_t* o2 = object_copy(o);
  assert_notnull(ERROR, o2, "Object allocation failure.");
  object_destroy(o);
  assert_false(
    ERROR,
    strcmp(object_data(o2), "Test"),
    "Object data failure."
  );
  object_destroy(o2);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t object_data_test() {
  char str[] = "Test string";
  object_t* o = object_create(str_type, str, false);
  assert_notnull(ERROR, o, "Object allocation failure.");
  assert_equal(ERROR, str, object_data(o), "Object data failure.");
  object_destroy(o);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t object_data_type_test1() {
  object_t* o = object_create(str_type, NULL, false);
  assert_notnull(ERROR, o, "Object allocation failure.");
  assert_equal(
    ERROR,
    str_type,
    object_data_type(o),
    "Object data type failure."
  );
  object_destroy(o);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t object_data_type_test2() {
  object_t* o = object_create(ptr_type, NULL, false);
  assert_notnull(ERROR, o, "Object allocation failure.");
  assert_equal(
    ERROR,
    ptr_type,
    object_data_type(o),
    "Object data type failure."
  );
  object_destroy(o);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t object_identifier_test1() {
  object_t* o = object_create(str_type, NULL, false);
  assert_notnull(ERROR, o, "Object allocation failure.");
  assert_false(
    ERROR,
    strcmp(object_identifier(o), "str"),
    "Object type failure."
  );
  object_destroy(o);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t object_identifier_test2() {
  object_t* o = object_create(ptr_type, NULL, false);
  assert_notnull(ERROR, o, "Object allocation failure.");
  assert_false(
    ERROR,
    strcmp(object_identifier(o), "ptr"),
    "Object type failure."
  );
  object_destroy(o);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t object_repr_test() {
  char* str = (char*)malloc(sizeof(*str) * 10);
  strcpy(str, "Test");
  object_t* o = object_create(str_type, str, true);
  assert_notnull(ERROR, o, "Object allocation failure.");
  rope_t* r = object_repr(o);
  char* repr = rope_str(r);
  assert_false(
    ERROR,
    strcmp("\"Test\"", repr),
    "Object repr failure."
  );
  free(repr);
  rope_destroy(r);
  object_destroy(o);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t object_hash_test1() {
  char* str1 = (char*)malloc(sizeof(*str1) * 10);
  char* str2 = (char*)malloc(sizeof(*str2) * 10);
  strcpy(str1, "Test");
  strcpy(str2, "Test");
  object_t* o1 = object_create(str_type, str1, true);
  object_t* o2 = object_create(str_type, str2, true);
  assert_notnull(ERROR, o1, "Object allocation failure.");
  assert_notnull(ERROR, o2, "Object allocation failure.");
  assert_equal(
    ERROR,
    object_hash(o1),
    object_hash(o2),
    "Object hash failure."
  );
  object_destroy(o1);
  object_destroy(o2);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t object_hash_test2() {
  char* str1 = (char*)malloc(sizeof(*str1) * 10);
  char* str2 = (char*)malloc(sizeof(*str2) * 10);
  strcpy(str1, "Test");
  strcpy(str2, "Tfst");
  object_t* o1 = object_create(str_type, str1, true);
  object_t* o2 = object_create(str_type, str2, true);
  assert_notnull(ERROR, o1, "Object allocation failure.");
  assert_notnull(ERROR, o2, "Object allocation failure.");
  assert_notequal(
    ERROR,
    object_hash(o1),
    object_hash(o2),
    "Object hash failure."
  );
  object_destroy(o1);
  object_destroy(o2);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t object_cmp_test1() {
  char* str1 = (char*)malloc(sizeof(*str1) * 10);
  char* str2 = (char*)malloc(sizeof(*str2) * 10);
  strcpy(str1, "Test");
  strcpy(str2, "Tfst");
  object_t* o1 = object_create(str_type, str1, true);
  object_t* o2 = object_create(str_type, str2, true);
  assert_notnull(ERROR, o1, "Object allocation failure.");
  assert_notnull(ERROR, o2, "Object allocation failure.");
  assert_true(
    ERROR,
    object_cmp(o1, o2) < 0,
    "Object cmp failure."
  );
  object_destroy(o1);
  object_destroy(o2);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t object_cmp_test2() {
  char* str1 = (char*)malloc(sizeof(*str1) * 10);
  char* str2 = (char*)malloc(sizeof(*str2) * 10);
  strcpy(str1, "Test");
  strcpy(str2, "Tdst");
  object_t* o1 = object_create(str_type, str1, true);
  object_t* o2 = object_create(str_type, str2, true);
  assert_notnull(ERROR, o1, "Object allocation failure.");
  assert_notnull(ERROR, o2, "Object allocation failure.");
  assert_true(
    ERROR,
    object_cmp(o1, o2) > 0,
    "Object cmp failure."
  );
  object_destroy(o1);
  object_destroy(o2);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t object_cmp_test3() {
  char* str1 = (char*)malloc(sizeof(*str1) * 10);
  char* str2 = (char*)malloc(sizeof(*str2) * 10);
  strcpy(str1, "Test");
  strcpy(str2, "Test");
  object_t* o1 = object_create(str_type, str1, true);
  object_t* o2 = object_create(str_type, str2, true);
  assert_notnull(ERROR, o1, "Object allocation failure.");
  assert_notnull(ERROR, o2, "Object allocation failure.");
  assert_true(
    ERROR,
    object_cmp(o1, o2) == 0,
    "Object cmp failure."
  );
  object_destroy(o1);
  object_destroy(o2);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t object_cmp_test4() {
  const type_t* null_test_type = &(type_t){
    .identifier = "null test",
    .destroy = NULL,
    .repr = NULL,
    .hash = NULL,
    .cmp = NULL
  };
  object_t* o1 = object_create(null_test_type, (void*)0x38, false);
  object_t* o2 = object_create(str_type, (void*)0x12, false);
  assert_notnull(ERROR, o1, "Object allocation failure.");
  assert_notnull(ERROR, o2, "Object allocation failure.");
  assert_true(
    ERROR,
    object_cmp(o1, o2) > 0,
    "Object cmp failure."
  );
  object_destroy(o1);
  object_destroy(o2);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t object_cmp_test5() {
  const type_t* null_test_type = &(type_t){
    .identifier = "null test",
    .destroy = NULL,
    .repr = NULL,
    .hash = NULL,
    .cmp = NULL
  };
  object_t* o1 = object_create(str_type, (void*)0x78, false);
  object_t* o2 = object_create(null_test_type, (void*)0x134, false);
  assert_notnull(ERROR, o1, "Object allocation failure.");
  assert_notnull(ERROR, o2, "Object allocation failure.");
  assert_true(
    ERROR,
    object_cmp(o1, o2) < 0,
    "Object cmp failure."
  );
  object_destroy(o1);
  object_destroy(o2);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t object_cmp_test6() {
  const type_t* null_test_type = &(type_t){
    .identifier = "null test",
    .destroy = NULL,
    .repr = NULL,
    .hash = NULL,
    .cmp = NULL
  };
  object_t* o1 = object_create(null_test_type, (void*)0x753, false);
  object_t* o2 = object_create(str_type, (void*)0x753, false);
  assert_notnull(ERROR, o1, "Object allocation failure.");
  assert_notnull(ERROR, o2, "Object allocation failure.");
  assert_true(
    ERROR,
    object_cmp(o1, o2) == 0,
    "Object cmp failure."
  );
  object_destroy(o1);
  object_destroy(o2);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t object_print_test() {
  object_t* o = object_create(str_type, "Test Text", false);
  assert_notnull(ERROR, o, "Object allocation failure.");

  size_t size = 100;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  __object_print(stream, o);
  fclose(stream);
  assert_false(
    ERROR,
    strcmp(buf, "\"Test Text\"\n"),
    "NULL print object failure."
  );
  free(buf);
  object_destroy(o);
  return EXIT_SUCCESS;
}
