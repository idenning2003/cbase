#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "g_test.h"
#include "assert.h"

uint8_t global_line_check(char* filename) {
  FILE* fp = fopen(filename, "r");
  assert_notnull(ERROR, fp, "Could not open file.");

  text_t* t = text_create();
  size_t col = 1;
  size_t row = 1;
  int c;
  char temp;
  uint8_t err = 0;
  while((c = getc(fp)) != EOF) {
    if (c == '\n') {
      if (
        !text_at(t, &temp, col - 2) &&
        (temp == ' ' || temp == '\t')
      ) {
        err |= __assert_fail(
          ERROR,
          "Trailing whitespace.",
          filename,
          NULL,
          row,
          col
        );
      }
      if (!text_at(t, &temp, 0) && temp == '#') {
        char* line = text_tostring(t);
        if (!strncmp(line, "#include", 8)) {
          __assert_true(
            ERROR,
            (line[8] == ' ' && line[9] == '"') ||
            (line[8] == ' ' && line[9] == '<'),
            "Include does not follow standards.",
            filename,
            NULL,
            row,
            col
          );
        }
        free(line);
      }
      if (col > 80)
        err |= __assert_fail(
          ERROR,
          "Line too long.",
          filename,
          NULL,
          row,
          col
        );

      col = 1;
      row++;
      assert_false(ERROR, text_clear(t), "Could not clear line.");
    } else {
      col++;
      assert_false(ERROR, text_append(t, (char)c), "Could not append line.");
    }
  }

  text_destroy(t);
  fclose(fp);
  return err;
}
