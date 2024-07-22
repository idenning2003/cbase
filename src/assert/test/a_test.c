#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "assert.h"
#include "a_internal.h"

FILE *fmemopen(void *__restrict, size_t, const char *__restrict);

__attribute__((test)) uint8_t assert_error_true_test1() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  uint8_t exit_code = __assert_true(
    stream,
    ERROR,
    true,
    "message",
    "file",
    "funcname",
    0, 0
  );
  fclose(stream);
  assert_false(
    ERROR,
    strcmp(buf, ""),
    "Assert true output failure."
  );
  free(buf);
  assert_false(
    ERROR,
    exit_code,
    "Assert exit code failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t assert_error_true_test2() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  uint8_t exit_code = __assert_true(
    stream,
    ERROR,
    false,
    "message",
    "file",
    "funcname",
    0, 0
  );
  fclose(stream);
  // Assert that it does output stuff, but I don't care what
  assert_true(
    ERROR,
    strcmp(buf, ""),
    "Assert true output failure."
  );
  free(buf);
  assert_true(
    ERROR,
    exit_code,
    "Assert exit code failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t assert_error_false_test1() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  uint8_t exit_code = __assert_false(
    stream,
    ERROR,
    false,
    "message",
    "file",
    "funcname",
    0, 0
  );
  fclose(stream);
  assert_false(
    ERROR,
    strcmp(buf, ""),
    "Assert false output failure."
  );
  free(buf);
  assert_false(
    ERROR,
    exit_code,
    "Assert exit code failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t assert_error_false_test2() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  uint8_t exit_code = __assert_false(
    stream,
    ERROR,
    true,
    "message",
    "file",
    "funcname",
    0, 0
  );
  fclose(stream);
  // Assert that it does output stuff, but I don't care what
  assert_true(
    ERROR,
    strcmp(buf, ""),
    "Assert false output failure."
  );
  free(buf);
  assert_true(
    ERROR,
    exit_code,
    "Assert exit code failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t assert_error_fail_test() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  uint8_t exit_code = __assert_fail(
    stream,
    ERROR,
    "message",
    "file",
    "funcname",
    0, 0
  );
  fclose(stream);
  // Assert that it does output stuff, but I don't care what
  assert_true(
    ERROR,
    strcmp(buf, ""),
    "Assert fail output failure."
  );
  free(buf);
  assert_true(
    ERROR,
    exit_code,
    "Assert exit code failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t assert_warning_true_test1() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  uint8_t exit_code = __assert_true(
    stream,
    WARNING,
    true,
    "message",
    "file",
    "funcname",
    0, 0
  );
  fclose(stream);
  assert_false(
    ERROR,
    strcmp(buf, ""),
    "Assert true output failure."
  );
  free(buf);
  assert_false(
    ERROR,
    exit_code,
    "Assert exit code failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t assert_warning_true_test2() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  uint8_t exit_code = __assert_true(
    stream,
    WARNING,
    false,
    "message",
    "file",
    "funcname",
    0, 0
  );
  fclose(stream);
  // Assert that it does output stuff, but I don't care what
  assert_true(
    ERROR,
    strcmp(buf, ""),
    "Assert true output failure."
  );
  free(buf);
  assert_false(
    ERROR,
    exit_code,
    "Assert exit code failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t assert_warning_false_test1() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  uint8_t exit_code = __assert_false(
    stream,
    WARNING,
    false,
    "message",
    "file",
    "funcname",
    0, 0
  );
  fclose(stream);
  assert_false(
    ERROR,
    strcmp(buf, ""),
    "Assert false output failure."
  );
  free(buf);
  assert_false(
    ERROR,
    exit_code,
    "Assert exit code failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t assert_warning_false_test2() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  uint8_t exit_code = __assert_false(
    stream,
    WARNING,
    true,
    "message",
    "file",
    "funcname",
    0, 0
  );
  fclose(stream);
  // Assert that it does output stuff, but I don't care what
  assert_true(
    ERROR,
    strcmp(buf, ""),
    "Assert false output failure."
  );
  free(buf);
  assert_false(
    ERROR,
    exit_code,
    "Assert exit code failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t assert_warning_fail_test() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  uint8_t exit_code = __assert_fail(
    stream,
    WARNING,
    "message",
    "file",
    "funcname",
    0, 0
  );
  fclose(stream);
  // Assert that it does output stuff, but I don't care what
  assert_true(
    ERROR,
    strcmp(buf, ""),
    "Assert fail output failure."
  );
  free(buf);
  assert_false(
    ERROR,
    exit_code,
    "Assert exit code failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t assert_note_true_test1() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  uint8_t exit_code = __assert_true(
    stream,
    NOTE,
    true,
    "message",
    "file",
    "funcname",
    0, 0
  );
  fclose(stream);
  assert_false(
    ERROR,
    strcmp(buf, ""),
    "Assert true output failure."
  );
  free(buf);
  assert_false(
    ERROR,
    exit_code,
    "Assert exit code failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t assert_note_true_test2() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  uint8_t exit_code = __assert_true(
    stream,
    NOTE,
    false,
    "message",
    "file",
    "funcname",
    0, 0
  );
  fclose(stream);
  // Assert that it does output stuff, but I don't care what
  assert_true(
    ERROR,
    strcmp(buf, ""),
    "Assert true output failure."
  );
  free(buf);
  assert_false(
    ERROR,
    exit_code,
    "Assert exit code failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t assert_note_false_test1() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  uint8_t exit_code = __assert_false(
    stream,
    NOTE,
    false,
    "message",
    "file",
    "funcname",
    0, 0
  );
  fclose(stream);
  assert_false(
    ERROR,
    strcmp(buf, ""),
    "Assert false output failure."
  );
  free(buf);
  assert_false(
    ERROR,
    exit_code,
    "Assert exit code failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t assert_note_false_test2() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  uint8_t exit_code = __assert_false(
    stream,
    NOTE,
    true,
    "message",
    "file",
    "funcname",
    0, 0
  );
  fclose(stream);
  // Assert that it does output stuff, but I don't care what
  assert_true(
    ERROR,
    strcmp(buf, ""),
    "Assert false output failure."
  );
  free(buf);
  assert_false(
    ERROR,
    exit_code,
    "Assert exit code failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t assert_note_fail_test() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  uint8_t exit_code = __assert_fail(
    stream,
    NOTE,
    "message",
    "file",
    "funcname",
    0, 0
  );
  fclose(stream);
  // Assert that it does output stuff, but I don't care what
  assert_true(
    ERROR,
    strcmp(buf, ""),
    "Assert fail output failure."
  );
  free(buf);
  assert_false(
    ERROR,
    exit_code,
    "Assert exit code failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t assert_no_funcname_test() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  uint8_t exit_code = __assert_fail(
    stream,
    ERROR,
    "message",
    "file",
    NULL,
    0, 0
  );
  fclose(stream);
  // Assert that it does output stuff, but I don't care what
  assert_true(
    ERROR,
    strcmp(buf, ""),
    "Assert fail output failure."
  );
  free(buf);
  assert_true(
    ERROR,
    exit_code,
    "Assert exit code failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t assert_invalid_level_test() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  uint8_t exit_code = __assert_fail(
    stream,
    (assert_level_t)55,
    "message",
    "file",
    NULL,
    0, 0
  );
  fclose(stream);
  assert_false(
    ERROR,
    strcmp(buf, ""),
    "Assert fail output failure."
  );
  free(buf);
  assert_false(
    ERROR,
    exit_code,
    "Assert exit code failure."
  );
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t assert_excerpt_test() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  // Excerpted line
  __assert_excerpt(stream, __FILE__, __LINE__ - 1);
  fclose(stream);
  char* iter = buf;
  for (; *iter != '|' && *iter; iter++);
  iter[21] = '\0';
  assert_false(
    ERROR,
    strcmp(iter, "|   // Excerpted line"),
    "Assert fail output failure."
  );
  free(buf);
  return EXIT_SUCCESS;
}

__attribute__((test)) uint8_t assert_excerpt_test2() {
  size_t size = 200;
  char* buf = (char*)malloc(sizeof(*buf) * size);
  for (size_t i = 0; i < size; i++)
    buf[i] = '\0';
  FILE* stream = fmemopen(buf, size, "r+");
  __assert_excerpt(stream, __FILE__, 9999);
  fclose(stream);
  // Assert that it does output stuff, but I don't care what
  assert_true(
    ERROR,
    strcmp(buf, ""),
    "Assert fail output failure."
  );
  free(buf);
  return EXIT_SUCCESS;
}
