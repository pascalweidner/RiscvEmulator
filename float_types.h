#ifndef FLOAT_TYPES_H_H
#define FLOAT_TYPES_H_H
#include <assert.h>

typedef float float32_t;
static_assert(sizeof(float) == 4, "float is not 32-bit on this platform!");

#endif