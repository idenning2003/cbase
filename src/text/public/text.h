#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef void text_t;

text_t* text_create();
uint8_t text_destroy(text_t*);
uint8_t text_length(text_t*);
uint8_t text_front(text_t*);
uint8_t text_goto(text_t*, size_t);
uint8_t text_at(text_t*, char*, size_t);
uint8_t text_next(text_t*, char*);
bool text_has_next(text_t*);
uint8_t text_append(text_t*, char);
uint8_t text_concat_string(text_t*, const char*);
uint8_t text_concat_text(text_t*, text_t*);
uint8_t text_clear(text_t*);
uint8_t text_delete(text_t*, size_t);
char* text_tostring(text_t*);

text_t* text_totext(text_t*);
text_t* string_totext(char*);
int8_t text_cmp(text_t*, text_t*);
int8_t text_cmp_string(text_t*, char*);
uint8_t text_print(text_t*);

#endif
