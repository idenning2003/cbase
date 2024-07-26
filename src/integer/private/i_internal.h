#ifndef I_INTERNAL_H
#define I_INTERNAL_H

#include <stdbool.h>

#include "list.h"

#define _self ((__integer_t*)self)
#define _parts (_self->parts)
#define _sign (_self->sign)

typedef struct Integer {
  list_t* parts;
  bool sign;
} __integer_t;

/// @brief List type
extern const type_t* int_part_type;

#endif
