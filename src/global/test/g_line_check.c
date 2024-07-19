#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "g_test.h"
#include "assert.h"

uint8_t global_line_check(char* filename) {
  FILE* fp = fopen(filename, "r");
  assert_notnull(ERROR, fp, "Could not open file.");

  char str[82];
  for (size_t i = 0; i < sizeof(str) / sizeof(*str); i++)
    str[i] = '\0';
  size_t col = 1;
  size_t row = 1;
  int c;
  uint8_t err = 0;
  while((c = getc(fp)) != EOF) {
    if (c == '\n') {
      if ((col >= 2) && (str[col - 2] == ' ' || str[col - 2] == '\t')) {
        err |= __assert_fail(
          stderr,
          ERROR,
          "Trailing whitespace.",
          filename,
          NULL,
          row,
          col
        );
      }
      if (str[0] == '#') {
        if (!strncmp(str, "#include", 8)) {
          __assert_true(
            stderr,
            ERROR,
            (str[8] == ' ' && str[9] == '"') ||
            (str[8] == ' ' && str[9] == '<'),
            "Include does not follow standards.",
            filename,
            NULL,
            row,
            col
          );
        }
      }
      if (col > 81) // 81 since newline character is included in count
        err |= __assert_fail(
          stderr,
          ERROR,
          "Line too long.",
          filename,
          NULL,
          row,
          col
        );

      col = 1;
      for (size_t i = 0; i < sizeof(str) / sizeof(*str); i++)
        str[i] = '\0';
      row++;
    } else {
      str[col - 1] = (char)c;
      col++;
    }
  }

  fclose(fp);
  return err;
}
