#include "./engine/engine.h"
#include <iostream>

struct particle
{
    uint32_t id;
};

enum par_id
{
    sand_id = 0xFFc2b280
};

class world_data
{
private:
    int _w, _h;

public:
    particle *p;
    world_data(int width, int height) : _w(width), _h(height), p(new particle[width * height])
    {
        for (int y = 0; y < _h; y++)
        {
            for (int x = 0; x < _w; x++)
            {
                particle t;
                t.id = 0;
                p[x + y * _w] = t;
            }
        }
    }

    void updateParticle(int x, int y, uint32_t id)
    {
        particle t;
        t.id = id;
        p[x + y * _w] = t;
    }

    uint32_t readParticle(int x, int y)
    {
        return p[x + y * _w].id;
    }

    ~world_data() { delete[] p; }
};
world_data w(640, 480);

bool leftMouseButtonDown;
int mouseX;
int mouseY;

void handle_Input(SDL_Event &event, bool &running)
{
    while (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
                leftMouseButtonDown = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            leftMouseButtonDown = true;
            SDL_GetMouseState(&mouseX, &mouseY);
        case SDL_MOUSEMOTION:
            if (leftMouseButtonDown)
            {
                SDL_GetMouseState(&mouseX, &mouseY);
            }
        }
    }
}

void updatesand(int x, int y, world_data &w)
{
    if (x >= 639 || y >= 479)
        return;
    uint32_t below = w.readParticle(x, y + 1);
    if (below == 0)
    {
        w.updateParticle(x, y, 0);
        w.updateParticle(x, y + 1, sand_id);
        return;
    }

    uint32_t below_left = w.readParticle(x - 1, y + 1);

    if (below_left == 0)
    {
        w.updateParticle(x, y, 0);
        w.updateParticle(x - 1, y + 1, sand_id);
        return;
    }

    uint32_t below_right = w.readParticle(x + 1, y + 1);

    if (below_right == 0)
    {
        w.updateParticle(x, y, 0);
        w.updateParticle(x + 1, y + 1, sand_id);
        return;
    }
}

engine *e = new engine(640, 480);
void copyTex()
{

    for (int i = 0; i < 480; i++)
    {
        for (int j = 0; j < 640; j++)
        {
            e->pixels[j + i * 640] = w.readParticle(j, i);
        }
    }
}

int main(int argc, char **argv)
{
    while (e->running)
    {

        if (leftMouseButtonDown)
        {
            w.updateParticle(mouseX, mouseY, sand_id);
        }
        e->update(handle_Input, copyTex);
        for (int y = 480 - 1; y > 0; y--)
        {
            for (int x = 1; x < 640; x++)
            {
                uint32_t id = w.readParticle(x, y);

                switch (id)
                {
                case sand_id:
                    updatesand(x, y, w);
                    break;

                default:
                    break;
                }
            }
        }
    }

    delete e;
    return 0;
}