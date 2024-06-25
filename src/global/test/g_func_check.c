#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "g_test.h"
#include "assert.h"

// TODO Implement
uint8_t global_func_check(char* filename) {
  size_t len = strlen(filename);
  char* ext = filename + len;
  while (*ext != '.' && ext != filename) ext--;
  if (strcmp(ext, ".c")) return EXIT_SUCCESS;

  FILE* fp = fopen(filename, "r");
  assert_notnull(ERROR, fp, "Could not open file.");

  text_t* t = text_create();
  size_t i = 0;
  int c, row = 1, col = 1;
  uint8_t err = 0;
  while ((c = getc(fp)) != EOF) {
    if (c == '\n') {
      row++;
      col = 1;
    } else {
      col++;
    }
    assert_false(ERROR, text_append(t, (char)c), "Could not append text.");
    i++;
  }
  //text_print(t);

  text_destroy(t);
  fclose(fp);
  return err;
}
