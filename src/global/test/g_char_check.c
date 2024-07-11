#include <stdio.h>

#include "g_test.h"
#include "assert.h"

uint8_t global_char_check(char* filename) {
  FILE* fp = fopen(filename, "r");
  assert_notnull(ERROR, fp, "Could not open file.");

  int last = '\n', c, row = 1, col = 1;
  uint8_t err = 0;
  while ((c = getc(fp)) != EOF) {
    if (c == '\n') {
      row++;
      col = 1;
    } else {
      col++;
    }
    if (c == '\r') {
      err |= __assert_fail(
        WARNING,
        "Carriage return character found.",
        filename,
        NULL,
        row,
        col
      );
    }
    if (c == '\t') {
      err |= __assert_fail(
        WARNING,
        "Tab character found.",
        filename,
        NULL,
        row,
        col
      );
    }
    last = c;
  }

  if (last != '\n') {
    err |= __assert_fail(
        WARNING,
        "Carriage return character not found at end of file.",
        filename,
        NULL,
        row,
        col
      );
  }

  fclose(fp);
  return err;
}
