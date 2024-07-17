#include "object.h"
#include "class.h"

#define _self             ((__object_t*)self)
#define _type             (_self->type)
#define _data             (_self->data)
#define _connect_destroy  (_self->connect_destroy)

typedef struct {
  const class_t* type;
  void* data;
  bool connect_destroy;
} __object_t;
