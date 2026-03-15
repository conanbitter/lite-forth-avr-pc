#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <stdio.h>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Minimal SDL3 App", 960, 640, SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    SDL_Surface* font_surface = SDL_LoadBMP("font.bmp");
    if (font_surface == NULL) {
        SDL_Log("Could not load font image! SDL_Error: %s\n", SDL_GetError());
    }
    SDL_Texture* font_texture = SDL_CreateTextureFromSurface(renderer, font_surface);
    SDL_DestroySurface(font_surface);

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, font_texture, NULL, NULL);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(font_texture);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}