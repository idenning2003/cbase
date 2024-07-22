#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>

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
  #define WEXITSTATUS(p) 0
#endif

int main(UNUSED int argc, UNUSED char* argv[]) {
  char dir[] = "bin/tests/";
  DIR *dr = opendir(dir);
  assert_notnull(ERROR, dr, "Could open directory bin/tests/");

  struct dirent *de;
  int err = 0, status = 0;
  char exe[PATH_MAX];

  // Check the public ssh key hash to disable valgrind on raspberry pi runner
  // TODO change this method to something more sensible or get a better runner.
  char hash_cmd[] = "[ "
                        "$(md5sum /home/dev/.ssh/id_rsa.pub | "
                        "awk '{print $1}') "
                      "== "
                        "'9c55f619dba0eb1c0ccb6cc790b842dc' "
                    "]";
  char prefix[] = "timeout 5 "
                    "valgrind "
                      "-q "
                      "--leak-check=full "
                      "--track-origins=yes --error-exitcode=117 ";
  char suffix[] = " 2>&1";
  if (!WEXITSTATUS(system(hash_cmd)))
    prefix[10] = '\0';
  int prefix_len = strlen(prefix);
  char full[
    prefix_len +
    sizeof(exe) / sizeof(*exe) +
    sizeof(suffix) / sizeof(*suffix) +
    1
  ];
  strcpy(full, prefix);
  while ((de = readdir(dr)) != NULL) {
    if (de->d_type == DT_REG) {
      strcpy(exe, dir);
      sprintf(exe + sizeof(dir)/sizeof(*dir) - 1, "%s", de->d_name);
      int exe_len = strlen(basename(exe));
      for (int i = 0; i < (69 - exe_len) / 2; i++)
        printf("-");
      printf(" TESTING: %s ", de->d_name);
      for (int i = 0; i < (69 - exe_len) / 2 + (69 - exe_len) % 2; i++)
        printf("-");
      printf("\n");
      strcpy(full + prefix_len, exe);
      strcpy(full + prefix_len + strlen(exe), suffix);
      FILE *fp = popen(full, "r");
      if (fp == NULL) {
        printf("Error: Alias connection failure.");
        return 1;
      }
      int c;
      while ((c = fgetc(fp)) != EOF) {
        putc(c, stdout);
      }
      status = WEXITSTATUS(pclose(fp));
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
    strcpy(exe, RED " TESTING FAILURE " RST);
  else
    strcpy(exe, GRN " TESTING SUCESSFUL " RST);

  int len = strlen(exe) - strlen(RED) - strlen(RST);
  for (int i = 0; i < (80 - len) / 2; i++)
    printf("-");
  printf("%s", exe);
  for (int i = 0; i < (80 - len) / 2 + (80 - len) % 2; i++)
    printf("-");
  printf("\n");

  closedir(dr);
  return err;
}
