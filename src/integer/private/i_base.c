#include <stdlib.h>
#include <math.h>

#include "integer.h"
#include "i_internal.h"
#include "list.h"

integer_t* integer_create_int(int64_t val) {
  integer_t* self = (integer_t*)malloc(sizeof(*_self));
  _parts = list_create(int_part_type, true);
  _sign = val < 0;
  if (_sign) {
    val -= 1;
    val = ~val;
  }

  int64_t* int_part = (int64_t*)malloc(sizeof(*int_part));
  *int_part = val;
  list_append(_parts, int_part);
  int_part = (int64_t*)malloc(sizeof(*int_part));
  if (val == 1)
    *int_part = UINT64_MAX;
  else
    *int_part = 0;
  list_append(_parts, int_part);

  return self;
}

void integer_destroy(integer_t* self) {
  list_destroy(_parts);
  free(self);
}

// integer_t* integer_copy(integer_t*) {

// }

// TODO Implement for negative signs
void integer_add_integer(integer_t* self, integer_t* other) {
  uint64_t* a;
  uint64_t* b;
  bool carry = false;
  list_tail(_parts);
  list_tail((((__integer_t*)other)->parts));
  while (
    !list_prev(_parts, (list_item_t **)&a) &&
    !list_prev((((__integer_t*)other)->parts), (list_item_t **)&b)
  ) {
    printf("A:%lx B:%lx C:%d\n", *a, *b, carry);
    bool ovf = *a > UINT64_MAX - *b || *a + *b > UINT64_MAX - carry;
    *a += *b + carry;
    if (ovf)
      carry = 1;
    else
      carry = 0;
  }
  printf("%d\n", carry);
}

void __integer_print(FILE* f, integer_t* self) {
  char* str = (char*)malloc(sizeof(*str) * sizeof(uint64_t) * 2 * list_size(_parts) + 1);
  uint64_t* val;
  if (_sign)
    fprintf(f, "-");
  list_head(_parts);
  while(!list_next(_parts, (list_item_t **)&val)) {
    if (list_has_prev(_parts))
      fprintf(f, "%016lx", *val);
    else
      fprintf(f, "%lx", *val);
  }
  fprintf(f, "\n");
  free(str);
  // __list_print(f, _parts);
  // const int int_part_str_size = (int)ceill(log10(pow(2, sizeof(uint64_t) * 8)));
  // const size_t parts_len = list_size(_parts);
  // size_t i, strlen = parts_len * int_part_str_size + _sign;
  // size_t str_index = strlen;
  // char* str = (char*)malloc(sizeof(*str) * (strlen + 1));
  // str[str_index + 1] = '\0';

  // uint64_t* iter;
  // uint64_t* parts_mod = (uint64_t*)malloc(sizeof(*parts_mod) * parts_len);
  // uint64_t* parts_div = (uint64_t*)malloc(sizeof(*parts_mod) * parts_len);
  // i = 0;
  // list_head(_parts);
  // while (i < parts_len) {
  //   list_next(_parts, (list_item_t**)&iter);
  //   parts_div[i++] = *iter;
  // }

  // bool is_empty = false;
  // while (!is_empty) {
  //   for (i = 0; i < parts_len; i++)
  //     parts_mod[i] = parts_div[i] % 10;

  //   is_empty = true;
  //   for (i = 0; i < parts_len; i++) {
  //     parts_div[i] /= 10;
  //     if (i)
  //       parts_div[i] += parts_mod[i - 1] * 0x1999999999999999;
  //     if (parts_div[i])
  //       is_empty = false;
  //   }

  //   uint8_t mod = 0;
  //   for (i = 0; i < parts_len; i++) {
  //     mod = parts_mod[i] + (6 * mod) % 10;
  //   }
  //   mod %= 10;
  //   str[str_index--] = mod + '0';
  // }

  // if (_sign)
  //   str[str_index--] = '-';

  // fprintf(f, "%s\n", str + str_index + 1);
  // free(str);
  // free(parts_mod);
  // free(parts_div);
}
