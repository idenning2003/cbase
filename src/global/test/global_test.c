#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "list.h"
#include "global.h"
#include "g_test.h"

__attribute__((test)) int global_test() {
  char cwd[PATH_MAX];
  assert_notnull(ERROR, getcwd(cwd, sizeof(cwd)), "Could not get CWD.");

  list_t* files = list_create(str_type, true);
  uint8_t err = global_findall_files(files, cwd);
  assert_null(ERROR, err, "Could not find test executables.");

  char* file;
  list_head(files);
  while (!list_next(files, (void**)&file)) {
    size_t len = strlen(file);
    char* ext = file + len;
    while (*ext != '.' && ext != file) ext--;
    bool is_cfile = !strcmp(ext, ".c");
    bool is_hfile = !strcmp(ext, ".h");
    if (is_cfile || is_hfile) {
      err |= global_line_check(file);
      err |= global_char_check(file);
      err |= global_func_check(file);
    }
  }

  list_destroy(files);
  return err;
}

uint8_t global_findall_files(list_t* files, char* path) { // Hello world
  size_t pathlen = strlen(path);
  DIR *dr = opendir(path);
  assert_notnull(ERROR, dr, "Could open directory.");

  struct dirent *de;
  while ((de = readdir(dr)) != NULL) {
    if (de->d_type == DT_REG) {
      char* s = (char*)malloc(sizeof(*s) * PATH_MAX);
      strcpy(s, path);
      sprintf(s + pathlen, "/%s", de->d_name);
      list_append(files, s);
    } else if (de->d_type == DT_DIR) {
      if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
        continue;
      sprintf(path + pathlen, "/%s", de->d_name);
      global_findall_files(files, path);
      path[pathlen] = '\0';
    }
  }
  closedir(dr);
  return EXIT_SUCCESS;
}
