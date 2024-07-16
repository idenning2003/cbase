#include "object.h"
#include "class.h"

#define _self ((__object_t*)self)
#define _type (_self->type)
#define _data (_self->data)

typedef struct {
  const class_t* type;
  void* data;
} __object_t;
