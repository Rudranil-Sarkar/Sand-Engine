#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <stdint.h>
#include <SDL2/SDL.h>

class engine
{
private:
    int width, height;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *tex;

public:
    SDL_Event event;
    uint32_t *pixels;
    bool running = true;
    float gravity = 3.f;

public:
    engine(int _w, int _h);
    void update(void (*handle_Input)(SDL_Event &, bool &), void (*copyTex)());
    ~engine();
};

#endif