#ifndef ASSERT_H
#define ASSERT_H

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/**
 * @brief Assert value is true
 *
 * @param l Error level
 * @param v Boolean value
 * @param s Error string
 */
#define assert_true(l, v, s) ({ \
  uint8_t _a_temp_ = __assert_true( \
    stderr, \
    l, \
    v, \
    s, \
    __FILE__, \
    __PRETTY_FUNCTION__, \
    __LINE__, \
    0 \
  ); \
  if (_a_temp_) \
    return _a_temp_; \
})

/**
 * @brief Assert value is false
 *
 * @param l Error level
 * @param v Boolean value
 * @param s Error string
 */
#define assert_false(l, v, s) ({ \
  uint8_t _a_temp_ = __assert_false( \
    stderr, \
    l, \
    v, \
    s, \
    __FILE__, \
    __PRETTY_FUNCTION__, \
    __LINE__, \
    0 \
  ); \
  if (_a_temp_) \
    return _a_temp_; \
})

/**
 * @brief Assert a failure
 *
 * @param l Error level
 * @param s Error string
 */
#define assert_fail(l, s) ({ \
  uint8_t _a_temp_ = __assert_fail( \
    stderr, \
    l, \
    s, \
    __FILE__, \
    __PRETTY_FUNCTION__, \
    __LINE__, \
    0 \
  ); \
  if (_a_temp_) \
    return _a_temp_; \
})

/**
 * @brief Assert values are equal
 *
 * @param l Error level
 * @param v1 First value
 * @param v2 Second value
 * @param s Error string
 */
#define assert_equal(l, v1, v2, s) ({ \
  if (v1 != v2) { \
    uint8_t _a_temp_ = __assert_fail( \
    stderr, \
      l, \
      s, \
      __FILE__, \
      __PRETTY_FUNCTION__, \
      __LINE__, \
      0 \
    ); \
    if (_a_temp_) \
      return _a_temp_; \
  } \
}) \

/**
 * @brief Assert values are not equal
 *
 * @param l Error level
 * @param v1 First value
 * @param v2 Second value
 * @param s Error string
 */
#define assert_notequal(l, v1, v2, s) ({ \
  if (v1 == v2) { \
    uint8_t _a_temp_ = __assert_fail( \
    stderr, \
      l, \
      s, \
      __FILE__, \
      __PRETTY_FUNCTION__, \
      __LINE__, \
      0 \
    ); \
    if (_a_temp_) \
      return _a_temp_; \
  } \
}) \

/**
 * @brief Assert value is NULL
 *
 * @param l Error level
 * @param v Value
 * @param s Error string
 */
#define assert_null(l, v, s) ({ \
  if (v) { \
    uint8_t _a_temp_ = __assert_fail( \
    stderr, \
      l, \
      s, \
      __FILE__, \
      __PRETTY_FUNCTION__, \
      __LINE__, \
      0 \
    ); \
    if (_a_temp_) \
      return _a_temp_; \
  } \
}) \

/**
 * @brief Assert value is not NULL
 *
 * @param l Error level
 * @param v Value
 * @param s Error string
 */
#define assert_notnull(l, v, s) ({ \
  if (!v) { \
    uint8_t _a_temp_ = __assert_fail( \
    stderr, \
      l, \
      s, \
      __FILE__, \
      __PRETTY_FUNCTION__, \
      __LINE__, \
      0 \
    ); \
    if (_a_temp_) \
      return _a_temp_; \
  } \
}) \

typedef enum __attribute__((__packed__)) {
  ERROR,
  WARNING,
  NOTE
} assert_level_t;


uint8_t __assert_true(
  FILE*,
  assert_level_t,
  bool,
  const char*,
  const char*,
  const char*,
  size_t,
  size_t
);

uint8_t __assert_false(
  FILE*,
  assert_level_t,
  bool,
  const char*,
  const char*,
  const char*,
  size_t,
  size_t
);

uint8_t __assert_fail(
  FILE*,
  assert_level_t,
  const char*,
  const char*,
  const char*,
  size_t,
  size_t
);

#endif
