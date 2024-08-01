#include <unistd.h>

#include "list.h"

uint8_t global_findall_files(list_t* files, char* path);
uint8_t global_line_check(char* filename);
uint8_t global_char_check(char* filename);
uint8_t global_func_check(char* filename);
