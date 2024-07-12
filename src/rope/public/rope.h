#ifndef ROPE_H
#define ROPE_H

typedef void rope_t;

rope_t* rope_create_with(const char*);
void rope_destroy(rope_t*);
char* rope_str(rope_t*);

#endif
