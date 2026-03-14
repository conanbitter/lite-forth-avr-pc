#ifndef INCLUDE_H
#define INCLUDE_H

#include <stdint.h>

#define INPUT_MAXLENGTH (80)
#define WORD_MAXLENGTH (32)

typedef uint8_t fchar;

typedef int16_t fcell;
typedef uint16_t fucell;

typedef struct WordHead
{
    struct WordHead* link;
    fchar flags;
    fchar len;
    char name[WORD_MAXLENGTH];
    fucell cfa;
} WordHead;


#endif