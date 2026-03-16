#include <stdio.h>
#include <inttypes.h>
#include "common.h"
#include "textmode.h"

#define INITIAL_SCALE (2)

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Texture* frame_texture;

Uint64 last_time = 0;

int main() {
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Forth", FRAME_WIDTH * INITIAL_SCALE, FRAME_HEIGHT * INITIAL_SCALE, SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, NULL);

    frame_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, FRAME_WIDTH, FRAME_HEIGHT);
    SDL_SetTextureScaleMode(frame_texture, SDL_SCALEMODE_NEAREST);

    tm_init(renderer);

    last_time = SDL_GetTicksNS();

    tm_print("This is a test message.");

    bool quit = false;
    SDL_Event e;
    SDL_StartTextInput(window);
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type)
            {
            case SDL_EVENT_QUIT:
                quit = true;
                break;

            case SDL_EVENT_TEXT_INPUT:
                if (e.text.text[1] == '\0') {
                    tm_putc(e.text.text[0]);
                }
                break;

            case SDL_EVENT_KEY_DOWN:
                if (e.key.key == SDLK_BACKSPACE) {
                    tm_putc('\b');
                }
                if (e.key.key == SDLK_RETURN) {
                    tm_putc('\r');
                    tm_putc('\n');
                }
                if (e.key.key == SDLK_TAB) {
                    tm_putc('\t');
                }
                break;

            default:
                break;
            }
        }

        Uint64 dt = SDL_GetTicksNS() - last_time;

        tm_update(dt);

        last_time = SDL_GetTicksNS();


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_SetRenderTarget(renderer, frame_texture);
        SDL_RenderClear(renderer);
        tm_draw(renderer);
        SDL_SetRenderTarget(renderer, NULL);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, frame_texture, NULL, NULL);

        SDL_RenderPresent(renderer);
    }

    tm_free();
    SDL_DestroyTexture(frame_texture);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}