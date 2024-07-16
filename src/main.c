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


  for (int i = 0; i < 2; i++) {
    printf("%d\n", strcmp("3", "1"));
  }

  return 0;
}
