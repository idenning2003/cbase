#include "assert.h"

void __assert_out(
  FILE*,
  assert_level_t,
  const char*,
  const char*,
  const char*,
  size_t,
  size_t
);

void __assert_excerpt(FILE*, const char* file, size_t line);
