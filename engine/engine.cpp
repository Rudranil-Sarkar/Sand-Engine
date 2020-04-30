#include "engine.h"

engine::engine(int _w, int _h)
    : width(_w), height(_h)
{
    pixels = new uint32_t[_w * _h];
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Sand Engine",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              width, height, 0);

    renderer = SDL_CreateRenderer(window, -1, 0);

    tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888,
                            SDL_TEXTUREACCESS_STATIC, width, height);
    memset(pixels, 0, width * height * sizeof(uint32_t));
}

engine::~engine()
{
    delete[] pixels;
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void engine::update(void (*handle_Input)(SDL_Event &, bool &))
{
    SDL_UpdateTexture(tex, NULL, pixels, width * sizeof(uint32_t));
    SDL_WaitEvent(&event);

    handle_Input(event, running);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, tex, NULL, NULL);
    SDL_RenderPresent(renderer);
}