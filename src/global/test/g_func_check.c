#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "g_test.h"
#include "assert.h"

// TODO Implement
uint8_t global_func_check(char* filename) {
  size_t len = strlen(filename);
  char* ext = filename + len;
  while (*ext != '.' && ext != filename) ext--;
  if (strcmp(ext, ".c"))
    return EXIT_SUCCESS;

  FILE* fp = fopen(filename, "r");
  assert_notnull(ERROR, fp, "Could not open file.");

  fclose(fp);
  return EXIT_SUCCESS;
}
