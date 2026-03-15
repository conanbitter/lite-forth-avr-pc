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

#define INITIAL_SCALE (2)

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Texture* font_texture;
SDL_Texture* frame_texture;

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

    SDL_Surface* font_surface = SDL_LoadBMP("font.bmp");
    if (font_surface == NULL) {
        SDL_Log("Could not load font image! SDL_Error: %s\n", SDL_GetError());
    }
    font_texture = SDL_CreateTextureFromSurface(renderer, font_surface);
    SDL_SetTextureScaleMode(font_texture, SDL_SCALEMODE_NEAREST);
    SDL_DestroySurface(font_surface);

    frame_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, FRAME_WIDTH, FRAME_HEIGHT);
    SDL_SetTextureScaleMode(frame_texture, SDL_SCALEMODE_NEAREST);

    text_src.w = CHAR_WIDTH;
    text_src.h = CHAR_HEIGHT;
    text_dst.w = CHAR_WIDTH;
    text_dst.h = CHAR_HEIGHT;

    char sample_text[80];
    memset(sample_text, 0, 80);
    sample_text[0] = '>';
    int caret = 1;
    int y = 1;

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
                    sample_text[caret] = e.text.text[0];
                    caret++;
                }
                break;

            case SDL_EVENT_KEY_DOWN:
                if (e.key.key == SDLK_BACKSPACE) {
                    if (caret > 1) {
                        caret--;
                        sample_text[caret] = '\0';
                    }
                }
                if (e.key.key == SDLK_RETURN) {
                    y++;
                }
                break;

            default:
                break;
            }
        }
        SDL_SetRenderTarget(renderer, frame_texture);
        SDL_RenderClear(renderer);
        //SDL_RenderTexture(renderer, font_texture, NULL, NULL);
        char* cur_char = sample_text;
        int x = 10;
        while (*cur_char != '\0') {
            drawLetter(*cur_char, x, y);
            x++;
            cur_char++;
        }
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, frame_texture, NULL, NULL);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(frame_texture);
    SDL_DestroyTexture(font_texture);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}