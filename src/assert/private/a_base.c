#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

#include "assert.h"
#include "a_internal.h"

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RST "\x1B[0m"

uint8_t _assert_true(
  assert_level_t l,
  bool v,
  const char* message,
  const char* file,
  const char* funcname,
  size_t row,
  size_t col
) {
  if (v == true)
    return EXIT_SUCCESS;
  char s[] = "Expected " GRN "true" RST ", but found " RED "false" RST ". ";
  char* full_message = (char*)malloc(
    sizeof(*full_message) * (
      sizeof(s) / sizeof(*s) + strlen(message) + 5
    )
  );
  strcpy(full_message, s);
  strcpy(full_message + sizeof(s) / sizeof(*s) - 1, message);
  _assert_out(l, full_message, file, funcname, row, col);
  free(full_message);
  return l == ERROR;
}

uint8_t _assert_false(
  assert_level_t l,
  bool v,
  const char* message,
  const char* file,
  const char* funcname,
  size_t row,
  size_t col
) {
  if (v == false)
    return EXIT_SUCCESS;
  char s[] = "Expected " GRN "false" RST ", but found " RED "true" RST ". ";
  char* full_message = (char*)malloc(
    sizeof(*full_message) * (
      sizeof(s) / sizeof(*s) + strlen(message) + 5
    )
  );
  strcpy(full_message, s);
  strcpy(full_message + sizeof(s) / sizeof(*s) - 1, message);
  _assert_out(l, full_message, file, funcname, row, col);
  free(full_message);
  return l == ERROR;
}

uint8_t _assert_fail(
  assert_level_t l,
  const char* message,
  const char* file,
  const char* funcname,
  size_t row,
  size_t col
) {
  _assert_out(l, message, file, funcname, row, col);
  return l == ERROR;
}

void _assert_excerpt(const char* file, size_t row) {
  FILE *fp = fopen(file,"r");
  if (fp == NULL)
    return;

  char linestr[21];
  sprintf(linestr, "%5zu", row);
  fprintf(stderr, "%s | ", linestr);

  int c = 0;
  size_t n = 1;
  while ((c = fgetc(fp)) != EOF) {
    if (n == row)
      fprintf(stderr, "%c", c);
    if (n > row)
      break;
    if (c == '\n')
      n++;
  }
  if (row == n)
    fprintf(stderr, "\n");
  fclose(fp);

  int len = strlen(linestr);
  for (int i = 0; i < len; i++)
    fprintf(stderr, " ");
  fprintf(stderr, " | \n");
}

void _assert_out(
  assert_level_t l,
  const char* message,
  const char* file,
  const char* funcname,
  size_t row,
  size_t col
) {
  char status[10];
  char color[10];
  switch (l) {
    case ERROR:
      strcpy(status, "error");
      strcpy(color, RED);
      break;
    case WARNING:
      strcpy(status, "warning");
      strcpy(color, YEL);
      break;
    case NOTE:
      strcpy(status, "note");
      strcpy(color, BLU);
      break;
  }
  if (funcname)
    fprintf(
      stderr,
      "%s:%zu:%zu %s%s: " RST "in " CYN "%s()" RST ". %s\n",
      file,
      row,
      col,
      color,
      status,
      funcname,
      message
    );
  else
    fprintf(
      stderr,
      "%s:%zu:%zu %s%s: " RST "%s\n",
      file,
      row,
      col,
      color,
      status,
      message
    );
  _assert_excerpt(file, row);
}
