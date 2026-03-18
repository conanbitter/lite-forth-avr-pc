#include <stdio.h>
#include <inttypes.h>
#include "common.h"
#include "textmode.h"

#define INITIAL_SCALE (2)

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Texture* frame_texture;

Uint64 last_time = 0;

const char* lorem_ipsum =
"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas ac egestas\r\n"
"tortor. Cras scelerisque ornare semper. Ut tortor nisl, imperdiet eget dolor\r\n"
"sed, tincidunt iaculis risus. Curabitur commodo, sem ut tristique congue, magna\r\n"
"enim vulputate diam, a facilisis magna dui eu mi. Quisque convallis, nisl vitae\r\n"
"porta porta, arcu risus fermentum nunc, quis aliquet lacus ante eu arcu.\r\n"
"Suspendisse fermentum massa lacus, eget convallis ligula blandit et.\r\n"
"Suspendisse a lorem mattis, tempus nunc sed, laoreet purus. Nulla eget enim\r\n"
"pellentesque, sagittis mauris ac, ullamcorper leo. Vestibulum vel ante id dui\r\n"
"accumsan blandit.\r\n"
"Donec iaculis nisi eu augue consequat, quis posuere neque pharetra. Duis\r\n"
"sollicitudin bibendum risus sit amet vehicula. Donec euismod accumsan turpis,\r\n"
"eget egestas libero. Cras sit amet tincidunt massa, non commodo nunc. Aliquam\r\n"
"tincidunt massa eu enim vehicula ornare. Cras lobortis porta tempus. Nam\r\n"
"pretium metus nisl. Cras congue a velit in maximus. Suspendisse urna ligula,\r\n"
"fermentum et tempus sed, ullamcorper sit amet sem. Morbi nec nulla et libero\r\n"
"scelerisque aliquam ut nec lorem. Nunc efficitur nisi ac ultrices pharetra.\r\n"
"Nullam efficitur ligula quis massa tempor facilisis.\r\n"
"Donec sollicitudin sed risus in tristique. Donec nec dignissim mi. Quisque\r\n"
"elementum felis enim, at tempor sapien hendrerit vitae. Praesent facilisis\r\n"
"risus et aliquet blandit. Nam nec ullamcorper dolor. Pellentesque ex ligula,\r\n"
"tempus sed nunc vitae, congue semper justo. Aenean auctor, ipsum vel fermentum\r\n"
"rhoncus, urna odio vulputate odio, sed cursus est ex accumsan nulla. Ut a\r\n"
"consequat nisi, eget consequat tortor.\r\n"
"Sed lacinia nec lectus id tempus. Vivamus in lectus semper, rhoncus ipsum id,\r\n"
"tristique mauris. Donec vulputate felis in elit fermentum porttitor. Sed\r\n"
"pretium consectetur consectetur.";

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

    tm_print(lorem_ipsum);
    tm_edit_start();

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
                    //tm_putc(e.text.text[0]);
                    tm_type(e.text.text[0]);
                }
                break;

            case SDL_EVENT_KEY_DOWN:
                switch (e.key.key)
                {
                case SDLK_RETURN:
                    tm_command(TMCMD_RETURN);
                    break;

                case SDLK_DELETE:
                    tm_command(TMCMD_DELETE);
                    break;

                case SDLK_BACKSPACE:
                    tm_command(TMCMD_BACKSPACE);
                    break;

                case SDLK_TAB:
                    tm_command(TMCMD_TAB);
                    break;

                case SDLK_HOME:
                    tm_command(TMCMD_HOME);
                    break;

                case SDLK_END:
                    tm_command(TMCMD_END);
                    break;

                case SDLK_INSERT:
                    tm_command(TMCMD_INSERT);
                    break;

                case SDLK_LEFT:
                    tm_command(TMCMD_LEFT);
                    break;

                case SDLK_RIGHT:
                    tm_command(TMCMD_RIGHT);
                    break;

                case SDLK_UP:
                    tm_command(TMCMD_UP);
                    break;

                case SDLK_DOWN:
                    tm_command(TMCMD_DOWN);
                    break;

                default:
                    break;
                }
                /*if (e.key.key == SDLK_BACKSPACE) {
                    tm_putc('\b');
                }
                if (e.key.key == ) {
                    //tm_putc('\r');
                    //tm_putc('\n');
                    tm_scroll(1);
                }
                if (e.key.key == SDLK_TAB) {
                    tm_putc('\t');
                }
                break;*/

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