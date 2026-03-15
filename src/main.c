#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <stdio.h>

#define FRAME_WIDTH (480)
#define FRAME_HEIGHT (320)

#define CHAR_WIDTH (6)
#define CHAR_HEIGHT (12)

#define TEXT_COLS (80)
#define TEXT_ROWS (26)
#define TEXT_XOFFSET ((FRAME_WIDTH-TEXT_COLS*CHAR_WIDTH)/2)
#define TEXT_YOFFSET ((FRAME_HEIGHT-TEXT_ROWS*CHAR_HEIGHT)/2)

#define FONT_COLS (16)
#define FONT_ROWS (16)

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Texture* font_texture;

SDL_FRect text_src;
SDL_FRect text_dst;

void drawLetter(char letter, int x, int y) {
    text_src.x = (letter % FONT_COLS) * CHAR_WIDTH;
    text_src.y = (letter / FONT_COLS) * CHAR_HEIGHT;

    text_dst.x = x * CHAR_WIDTH + TEXT_XOFFSET;
    text_dst.y = y * CHAR_HEIGHT + TEXT_YOFFSET;

    SDL_RenderTexture(renderer, font_texture, &text_src, &text_dst);
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Minimal SDL3 App", FRAME_WIDTH, FRAME_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, NULL);

    SDL_Surface* font_surface = SDL_LoadBMP("font.bmp");
    if (font_surface == NULL) {
        SDL_Log("Could not load font image! SDL_Error: %s\n", SDL_GetError());
    }
    font_texture = SDL_CreateTextureFromSurface(renderer, font_surface);
    SDL_DestroySurface(font_surface);

    text_src.w = CHAR_WIDTH;
    text_src.h = CHAR_HEIGHT;
    text_dst.w = CHAR_WIDTH;
    text_dst.h = CHAR_HEIGHT;

    char* sample_text = "This is a sample text.\x02";

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }
        SDL_RenderClear(renderer);
        //SDL_RenderTexture(renderer, font_texture, NULL, NULL);
        char* cur_char = sample_text;
        int x = 10;
        while (*cur_char != '\0') {
            drawLetter(*cur_char, x, 1);
            x++;
            cur_char++;
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(font_texture);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}