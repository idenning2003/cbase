#ifndef ROPE_H
#define ROPE_H

#include <stdint.h>

#define rope_print(self) ({ \
  __rope_print(stdout, self); \
}) \

typedef void rope_t;

rope_t* rope_create(const char*);
void rope_destroy(rope_t*);
rope_t* rope_copy(const rope_t*);
uint8_t rope_concat(rope_t*, const char*);
uint8_t rope_append(rope_t*, char);
uint8_t rope_add(rope_t*, const rope_t*);
char* rope_str(const rope_t*);

void __rope_print(FILE*, const rope_t*);

#endif
