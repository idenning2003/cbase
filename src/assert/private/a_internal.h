#include "assert.h"

void _assert_out(
  assert_level_t,
  const char*,
  const char*,
  const char*,
  size_t,
  size_t
);

void _assert_excerpt(const char* file, size_t line);
