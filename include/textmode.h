#ifndef TEXTMODE_H
#define TEXTMODE_H

#include <stdint.h>

#define TM_COLS (80)
#define TM_ROWS (26)

enum TmCommand {
    TMCMD_RETURN = 1,
    TMCMD_DELETE,
    TMCMD_BACKSPACE,
    TMCMD_TAB,
    TMCMD_HOME,
    TMCMD_END,
    TMCMD_INSERT,
    TMCMD_LEFT,
    TMCMD_RIGHT,
    TMCMD_UP,
    TMCMD_DOWN,
};

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
void tm_putc(char letter);
void tm_print(const char* text);
void tm_scroll(int line_count);

void tm_edit_start();
void tm_type(char letter);
void tm_command(enum TmCommand command);

#endif