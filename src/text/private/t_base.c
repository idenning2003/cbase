#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "text.h"
#include "t_internal.h"

#define _t ((text_internal_t*)t)
#define _front (_t->front)
#define _back (_t->back)
#define _iter (_t->iter)
#define _index (_t->index)
#define _length (_t->length)

text_t* text_create() {
  text_t* t = (text_t*)malloc(sizeof(*_t));
  _front = (text_node_internal_t*)malloc(sizeof(*_front));
  if (_t == NULL || _front == NULL)
    return NULL;
  _back = _front;
  _front->next = _back;
  _iter = _front;
  _index = -1;
  _length = 0;
  return t;
}

uint8_t text_destroy(text_t* t) {
  int err = text_clear(t);
  if (err)
    return err;
  free(_front);
  if (_front != _back)
    free(_back);
  free(_t);
  return EXIT_SUCCESS;
}

uint8_t text_length(text_t* t) {
  return _length;
}

uint8_t text_front(text_t* t) {
  _iter = _front;
  _index = -1;
  return EXIT_SUCCESS;
}

uint8_t __text_back(text_t* t) {
  _iter = _back;
  _index = _length;
  return EXIT_SUCCESS;
}

uint8_t text_goto(text_t* t, size_t index) {
  uint8_t err = 0;
  if (index >= _length)
    return ERANGE;
  if (_index > index) {
    err = text_front(t);
    if (err)
      return err;
  }
  if (_iter == _front) {
    err = text_next(t, NULL);
    if (err)
      return err;
  }
  while (_index < index) {
    text_next(t, NULL);
    if (err)
      return err;
  }
  return EXIT_SUCCESS;
}

uint8_t text_at(text_t* t, char* c, size_t index) {
  uint8_t err = text_goto(t, index);
  if (err)
    return err;
  if (c != NULL) *c = _iter->data;
  return EXIT_SUCCESS;
}

uint8_t text_next(text_t* t, char* c) {
  if (!text_has_next(t))
    return ERANGE;
  _iter = _iter->next;
  _index++;
  if (c != NULL) *c = _iter->data;
  return EXIT_SUCCESS;
}

bool text_has_next(text_t* t) {
  return _iter != _back;
}

uint8_t text_append(text_t* t, char c) {
  __text_back(t);
  text_node_internal_t* n = (text_node_internal_t*)malloc(sizeof(*n));
  if (n == NULL)
    return ENOMEM;
  n->data = c;
  _iter->next = n;
  _iter = n;
  _back = n;
  _index++;
  _length++;
  return EXIT_SUCCESS;
}

uint8_t text_concat_string(text_t* t, const char* c) {
  uint8_t err;
  for (; *c; c++) {
    err = text_append(t, *c);
    if (err)
      return err;
  }
  return EXIT_SUCCESS;
}

uint8_t text_concat_text(text_t* t, text_t* s2) {
  char c;
  uint8_t err = text_front(s2);
  if (err)
    return err;
  while (!text_next(s2, &c)) {
    err = text_append(t, c);
    if (err)
      return err;
  }
  return EXIT_SUCCESS;
}

uint8_t text_clear(text_t* t) {
  while (!text_delete(t, 0));
  return EXIT_SUCCESS;
}

uint8_t text_delete(text_t* t, size_t index) {
  uint8_t err;
  if (index >= _length)
    return ERANGE;
  if (index == 0)
    err = text_front(t);
  else
    err = text_goto(t, index - 1);
  if (err)
    return err;
  text_node_internal_t* n = _iter->next;
  _iter->next = n->next;
  if (n == _back)
    _back = _iter;
  free(n);
  _length--;
  return EXIT_SUCCESS;
}

char* text_tostring(text_t* t) {
  char* arr = (char*)malloc((_length + 1) + sizeof(*arr));
  char c;
  size_t index = 0;
  text_front(t);
  while (!text_next(t, &c)) {
    arr[index] = c;
    index++;
  }
  arr[index] = '\0';
  return arr;
}

text_t* text_totext(text_t* t) {
  return t;
}

text_t* string_totext(char* s) {
  text_t* t = text_create();
  text_concat_string(t, s);
  return t;
}

int8_t text_cmp(text_t*, text_t*);

int8_t text_cmp_string(text_t* t, char* s) {
  char c;
  text_front(t);
  while (1) {
    if (!*s) {
      if (!text_has_next(t))
        return 0;
      else
        return 1;
    }
    if (!text_next(t, &c) && *s != c)
      return c - *s;
    s++;
  }
}

uint8_t text_print(text_t* t) {
  char* s = text_tostring(t);
  printf("%s",s);
  free(s);
  return EXIT_SUCCESS;
}
