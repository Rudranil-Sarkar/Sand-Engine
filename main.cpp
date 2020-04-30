#include "./engine/engine.h"

void handle_Input(SDL_Event &e, bool &r)
{
    switch (e.type)
    {
    case SDL_QUIT:
        r = false;
        break;

    default:
        break;
    }
}
int main(int argc, char **argv)
{
    engine *e = new engine(640, 480);
    auto event = e->event;
    while (e->running)
    {
        e->update(handle_Input);
    }

    delete e;
    return 0;
}