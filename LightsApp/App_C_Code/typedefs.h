#ifndef TYPEDEFS_H
#define TYPEDEFS_H

typedef unsigned char UI_1_t;

typedef unsigned char UI_8_t;

typedef unsigned short UI_16_t;

typedef unsigned long UI_32_t;

typedef signed char SI_8_t;

typedef signed short SI_16_t;

typedef signed long SI_32_t;

typedef float F_32_t;

typedef double F_64_t;

typedef char STRING;

/* Return value for Blocks */

typedef UI_16_t       ARGUMENTS_t;                       /* function arguments */


typedef struct {
    UI_16_t x;     // x coordinate
    UI_16_t y;     // y coordinate
    UI_16_t z;     // z coordinate
} CONSTANT_LIGHT_OBJECT_t;

#endif