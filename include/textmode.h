#ifndef TEXTMODE_H
#define TEXTMODE_H

#include <stdint.h>

#define TM_COLS (80)
#define TM_ROWS (26)

typedef struct TmColor {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} TmColor;

const TmColor TM_COLOR_BLACK;
const TmColor TM_COLOR_WHITE;

void tm_init(SDL_Renderer* renderer);
void tm_free();
void tm_update(Uint64 dt);
void tm_draw(SDL_Renderer* renderer);
void tm_clrscr();


#endif