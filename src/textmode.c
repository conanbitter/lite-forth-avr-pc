#include "common.h"
#include "textmode.h"

#define CHAR_WIDTH (6)
#define CHAR_HEIGHT (12)

#define TEXT_XOFFSET ((FRAME_WIDTH-TM_COLS*CHAR_WIDTH)/2)
#define TEXT_YOFFSET ((FRAME_HEIGHT-TM_ROWS*CHAR_HEIGHT)/2)

#define FONT_COLS (16)
#define FONT_ROWS (16)

#define CURSOR_BLINK_PERIOD (500000000)

#define BUFFER_LENGTH (TM_COLS*TM_ROWS)

static const SDL_Rect CURSOR_SHAPE_INSERT = { .x = 1,.y = 10,.w = 5,.h = 2 };
static const SDL_Rect CURSOR_SHAPE_REPLACE = { .x = 1,.y = 0,.w = 5,.h = 12 };

const TmColor TM_COLOR_BLACK = { .r = 0,.g = 0,.b = 0 };
const TmColor TM_COLOR_WHITE = { .r = 255,.g = 255,.b = 255 };

typedef struct TmCell {
    int letter;
    TmColor fg_color;
} TmCell;

static SDL_Texture* font_texture;

static SDL_FRect text_src;
static SDL_FRect text_dst;
static SDL_FRect text_cursor;

static bool cursor_blink = true;
static Uint64 cursor_blink_time = 0;

static int cursor_x = 0;
static int cursor_y = 0;
static int cursor_pos = 0;
static SDL_Rect* cursor_shape = &CURSOR_SHAPE_INSERT;

static TmColor fg_color;

static TmCell buffer[BUFFER_LENGTH];

static void setCursorXY(int x, int y) {
    cursor_x = x;
    cursor_y = y;
    cursor_pos = x + y * TM_COLS;
    text_cursor.x = x * CHAR_WIDTH + TEXT_XOFFSET + cursor_shape->x;
    text_cursor.y = y * CHAR_HEIGHT + TEXT_YOFFSET + cursor_shape->y;
    cursor_blink = true;
    cursor_blink_time = 0;
}

static void setCursorPos(int pos) {
    cursor_pos = pos;
    cursor_x = pos % TM_COLS;
    cursor_y = pos / TM_COLS;
    text_cursor.x = cursor_x * CHAR_WIDTH + TEXT_XOFFSET + cursor_shape->x;
    text_cursor.y = cursor_y * CHAR_HEIGHT + TEXT_YOFFSET + cursor_shape->y;
    cursor_blink = true;
    cursor_blink_time = 0;
}

void tm_init(SDL_Renderer* renderer) {
    SDL_Surface* font_surface = SDL_LoadBMP("font.bmp");
    if (font_surface == NULL) {
        SDL_Log("Could not load font image! SDL_Error: %s\n", SDL_GetError());
    }
    font_texture = SDL_CreateTextureFromSurface(renderer, font_surface);
    SDL_SetTextureScaleMode(font_texture, SDL_SCALEMODE_NEAREST);
    SDL_DestroySurface(font_surface);

    text_src.w = CHAR_WIDTH;
    text_src.h = CHAR_HEIGHT;
    text_dst.w = CHAR_WIDTH;
    text_dst.h = CHAR_HEIGHT;
    text_cursor.w = cursor_shape->w;
    text_cursor.h = cursor_shape->h;

    fg_color = TM_COLOR_WHITE;

    tm_clrscr();
}

void tm_free() {
    SDL_DestroyTexture(font_texture);
}

void tm_update(Uint64 dt) {
    cursor_blink_time += dt;
    if (cursor_blink_time >= CURSOR_BLINK_PERIOD) {
        cursor_blink = !cursor_blink;
        cursor_blink_time -= CURSOR_BLINK_PERIOD;
    }
}

void tm_draw(SDL_Renderer* renderer) {
    TmCell* cell = buffer;

    text_dst.y = TEXT_YOFFSET;

    for (int y = 0;y < TM_ROWS;y++) {
        text_dst.x = TEXT_XOFFSET;
        for (int x = 0; x < TM_COLS; x++)
        {
            text_src.x = (cell->letter % FONT_COLS) * CHAR_WIDTH;
            text_src.y = (cell->letter / FONT_COLS) * CHAR_HEIGHT;

            SDL_SetTextureColorMod(font_texture, cell->fg_color.r, cell->fg_color.g, cell->fg_color.b);
            SDL_RenderTexture(renderer, font_texture, &text_src, &text_dst);

            text_dst.x += CHAR_WIDTH;
            cell++;
        }
        text_dst.y += CHAR_HEIGHT;
    }

    if (cursor_blink) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &text_cursor);
    }
}

void tm_clrscr() {
    for (int i = 0;i < BUFFER_LENGTH;i++) {
        buffer[i].letter = rand() % 255;
        buffer[i].fg_color = fg_color;
        buffer[i].fg_color.r = rand() % 255;
        buffer[i].fg_color.g = rand() % 255;
        buffer[i].fg_color.b = rand() % 255;
    }
    setCursorXY(0, 0);
}