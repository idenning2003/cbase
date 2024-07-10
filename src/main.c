#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "list.h"
#include "assert.h"

int main(int argc, char* argv[]) {
  for (int i = 0; i < argc; i++) {
    printf("%s ", argv[i]);
  }
  printf("\n");

  printf("%d\n", strcmp("A", "B"));

  return 0;
}
