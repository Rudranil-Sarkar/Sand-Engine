#include "./engine/engine.h"
#include "./particles/particles.h"
#include "worlddata.h"
#include <iostream>

Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;
double dt = 0;

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

engine *e = new engine(640, 480);
void copyTex()
{

    for (int i = 0; i < 480; i++)
    {
        for (int j = 0; j < 640; j++)
        {
            e->pixels[j + i * 640] = w.readParticle(j, i).id;
        }
    }
}

void updatesand(int x, int y, world_data &w)
{
    if (x >= 640 || y >= 480)
        return;

    particle this_particle = w.readParticle(x, y);
    particle below = w.readParticle(x, y + 1);

    this_particle.acc = e->gravity;
    this_particle.vel += this_particle.acc * dt / 2;
    if (below.id == 0)
    {
        int prevx = x;
        int prevy = y;
        for (int i = 1; i < this_particle.vel; i++)
        {
            if (w.readParticle(x, y + i).id != 0)
                break;

            if (x + i >= 640 || y + i >= 480)
                break;

            w.updateParticle(prevx, prevy, 0, 0, 0);
            w.updateParticle(x, y + i, sand_id, this_particle.vel, this_particle.acc);
            prevx = x;
            prevy = y + i;
        }
        return;
    }

    particle below_left = w.readParticle(x - 1, y + 1);

    if (below_left.id == 0)
    {
        int prevx = x;
        int prevy = y;
        for (int i = 1; i < this_particle.vel; i++)
        {
            if (w.readParticle(x - i, y + i).id != 0)
                break;

            if (x + i >= 640 || y + i >= 480 || x - i <= 0)
                break;

            w.updateParticle(prevx, prevy, 0, 0, 0);
            w.updateParticle(x - i, y + i, sand_id, this_particle.vel, this_particle.acc);
            prevx = x - i;
            prevy = y + i;
        }
        return;
    }

    particle below_right = w.readParticle(x + 1, y + 1);

    if (below_right.id == 0)
    {
        int prevx = x;
        int prevy = y;
        for (int i = 1; i < this_particle.vel; i++)
        {
            if (w.readParticle(x + i, y + i).id != 0)
                break;

            if (x + i >= 640 || y + i >= 480 || x - i <= 0)
                break;

            w.updateParticle(prevx, prevy, 0, 0, 0);
            w.updateParticle(x + i, y + i, sand_id, this_particle.vel, this_particle.acc);
            prevx = x + i;
            prevy = y + i;
        }
        return;
    }
}

int main(int argc, char **argv)
{
    while (e->running)
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        dt = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());

        if (leftMouseButtonDown)
        {
            w.updateParticle(mouseX, mouseY, sand_id, 0, 0);
            w.updateParticle(mouseX + 1, mouseY - 3, sand_id, 0, 0);
            w.updateParticle(mouseX + 2, mouseY - 2, sand_id, 0, 0);
            w.updateParticle(mouseX + 3, mouseY - 1, sand_id, 0, 0);
            w.updateParticle(mouseX - 1, mouseY + 3, sand_id, 0, 0);
            w.updateParticle(mouseX - 2, mouseY + 2, sand_id, 0, 0);
            w.updateParticle(mouseX - 3, mouseY + 1, sand_id, 0, 0);
        }
        e->update(handle_Input, copyTex);
        for (int y = 480 - 1; y > 0; y--)
        {
            for (int x = 1; x < 640; x++)
            {
                uint32_t id = w.readParticle(x, y).id;

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