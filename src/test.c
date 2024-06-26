#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include <stdlib.h>
#include <libgen.h>

#include "global.h"
#include "assert.h"

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define RST "\x1B[0m"

#ifndef DT_DIR
  #define DT_DIR 4
#endif

#ifndef DT_REG
  #define DT_REG 2
#endif

#ifndef PATH_MAX
  #define PATH_MAX 4096
#endif

#ifndef WEXITSTATUS
  #define WEXITSTATUS(l) 0
#endif

int main(UNUSED int argc, UNUSED char* argv[]) {
  char dir[] = "bin/tests";
  DIR *dr = opendir(dir);
  assert_notnull(ERROR, dr, "Could open directory bin/tests/");

  struct dirent *de;
  int err = 0, status = 0;
  char s[PATH_MAX];
  char cmd[] = "timeout 5 "
               "valgrind -q --leak-check=full --error-exitcode=117 ";
  char full[sizeof(s) / sizeof(*s) + sizeof(cmd) / sizeof(*cmd)];
  strcpy(full, cmd);
  while ((de = readdir(dr)) != NULL) {
    if (de->d_type == DT_REG) {
      strcpy(s, dir);
      sprintf(s + sizeof(dir)/sizeof(*dir) - 1, "/%s", de->d_name);
      int len = strlen(basename(s));
      for (int i = 0; i < (69 - len) / 2; i++)
        printf("-");
      printf(" TESTING: %s ", basename(s));
      for (int i = 0; i < (69 - len) / 2 + (69 - len) % 2; i++)
        printf("-");
      printf("\n");
      strcpy(full + sizeof(cmd) / sizeof(*cmd) - 1, s);
      status = WEXITSTATUS(system(full));
      if (status == 124)
        printf(RED "TIMEOUT ERROR\n" RST);
      else if (status == 117)
        printf(RED "MEMORY ERROR\n" RST);
      else if (status)
        printf(RED "EXIT CODE: %d\n" RST, status);
      err |= status;
    }
  }

  if (err)
    strcpy(s, RED " TESTING FAILURE " RST);
  else
    strcpy(s, GRN " TESTING SUCESSFUL " RST);

  int len = strlen(s) - strlen(RED) - strlen(RST);
  for (int i = 0; i < (80 - len) / 2; i++)
    printf("-");
  printf("%s", s);
  for (int i = 0; i < (80 - len) / 2 + (80 - len) % 2; i++)
    printf("-");
  printf("\n");

  closedir(dr);
  return err;
}
