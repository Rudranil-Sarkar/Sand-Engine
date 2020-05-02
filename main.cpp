#include "./engine/engine.h"
#include "./particles/particles.h"
#include "worlddata.h"
#include <iostream>

Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;
double dt = 0;

int width = 1280;
int height = 720;

world_data w(width, height);

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

engine *e = new engine(width, height);
void copyTex()
{

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            e->pixels[j + i * width] = w.readParticle(j, i).id;
        }
    }
}

void updatesand(int x, int y, world_data &w)
{
    if (x >= width || y >= height)
        return;

    particle this_particle = w.readParticle(x, y);
    particle below = w.readParticle(x, y + 1);

    this_particle.acc = e->gravity;
    if (this_particle.vel < 10)
        this_particle.vel += this_particle.acc * dt / 2;
    if (below.id == empty_id)
    {
        int prevx = x;
        int prevy = y;
        for (int i = 1; i < this_particle.vel; i++)
        {
            if (w.readParticle(x, y + i).id != 0)
                break;

            if (x + i >= width || y + i >= height)
                break;

            w.updateParticle(prevx, prevy, empty_id, 0, 0);
            w.updateParticle(x, y + i, sand_id, this_particle.vel, this_particle.acc);
            prevx = x;
            prevy = y + i;
        }
        return;
    }

    particle below_left = w.readParticle(x - 1, y + 1);

    if (below_left.id == empty_id)
    {
        int prevx = x;
        int prevy = y;
        for (int i = 1; i < this_particle.vel; i++)
        {
            if (w.readParticle(x - i, y + i).id != 0)
                break;

            if (x + i >= width || y + i >= height || x - i <= 0)
                break;

            w.updateParticle(prevx, prevy, empty_id, 0, 0);
            w.updateParticle(x - i, y + i, sand_id, this_particle.vel, this_particle.acc);
            prevx = x - i;
            prevy = y + i;
        }
        return;
    }

    particle below_right = w.readParticle(x + 1, y + 1);

    if (below_right.id == empty_id)
    {
        int prevx = x;
        int prevy = y;
        for (int i = 1; i < this_particle.vel; i++)
        {
            if (w.readParticle(x + i, y + i).id != empty_id)
                break;

            if (x + i >= width || y + i >= width || x - i <= 0)
                break;

            w.updateParticle(prevx, prevy, empty_id, 0, 0);
            w.updateParticle(x + i, y + i, sand_id, this_particle.vel, this_particle.acc);
            prevx = x + i;
            prevy = y + i;
        }
        return;
    }
}

void updatewater(int x, int y, world_data &w)
{
#define water_flow 30
    if (x >= width - 1 || y >= height - 1)
        return;

    particle this_particle = w.readParticle(x, y);
    particle below = w.readParticle(x, y + 1);

    this_particle.acc = e->gravity;
    if (this_particle.vel < 15)
        this_particle.vel += this_particle.acc * dt;

    if (below.id == empty_id)
    {
        int prevx = x;
        int prevy = y;

        for (int i = 1; i < this_particle.vel; i++)
        {
            if (w.readParticle(x, y + i).id != 0)
                break;
            if (x + i >= width - 1 || y + i >= height - 1)
                break;
            w.updateParticle(prevx, prevy, empty_id, 0, 0);
            w.updateParticle(x, y + i, water_id, this_particle.vel, this_particle.acc);
            prevx = x;
            prevy = y + i;
        }
        return;
    }
    particle below_left = w.readParticle(x - 1, y + 1);

    if (below_left.id == empty_id)
    {
        int prevx = x;
        int prevy = y;
        for (int i = 1; i < this_particle.vel; i++)
        {
            if (w.readParticle(x - i, y + i).id != 0)
                break;

            if (x + i >= width - 1 || y + i >= height - 1 || x - i <= 0)
                break;

            w.updateParticle(prevx, prevy, empty_id, 0, 0);
            w.updateParticle(x - i, y + i, water_id, this_particle.vel, this_particle.acc);
            prevx = x - i;
            prevy = y + i;
        }
        return;
    }

    particle below_right = w.readParticle(x + 1, y + 1);

    if (below_right.id == empty_id)
    {
        int prevx = x;
        int prevy = y;
        for (int i = 1; i < this_particle.vel; i++)
        {
            if (w.readParticle(x + i, y + i).id != empty_id)
                break;

            if (x + i >= width - 1 || y + i >= height - 1 || x - i <= 0)
                break;

            w.updateParticle(prevx, prevy, empty_id, 0, 0);
            w.updateParticle(x + i, y + i, water_id, this_particle.vel, this_particle.acc);
            prevx = x + i;
            prevy = y + i;
        }
        return;
    }

    particle left = w.readParticle(x - 1, y);

    if (left.id == empty_id)
    {
        int prevx = x;
        int prevy = y;
        for (int i = 1; i < water_flow; i++)
        {
            if (w.readParticle(x - i, y).id != empty_id)
                break;
            if (x + i >= width - 1 || x - i <= 0)
                break;

            w.updateParticle(prevx, prevy, empty_id);
            w.updateParticle(x - i, y, water_id, 0, 0);
            prevx = x - i;
            prevy = y;
        }
        return;
    }
    particle right = w.readParticle(x + 1, y);

    if (right.id == empty_id)
    {
        int prevx = x;
        int prevy = y;
        for (int i = 1; i < water_flow; i++)
        {
            if (w.readParticle(x + i, y).id != empty_id)
                break;
            if (x + i >= width - 1 || x - i <= 0)
                break;

            w.updateParticle(prevx, prevy, empty_id);
            w.updateParticle(x + i, y, water_id, 0, 0);
            prevx = x + i;
            prevy = y;
        }
        return;
    }
}

int main(int argc, char **argv)
{
    int frame_number = 0;
    while (e->running)
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        dt = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());

        if (leftMouseButtonDown)
        {
            w.updateParticle(mouseX, mouseY, water_id, 0, 0);
            w.updateParticle(mouseX + 1, mouseY - 3, water_id, 0, 0);
            w.updateParticle(mouseX + 2, mouseY - 2, water_id, 0, 0);
            w.updateParticle(mouseX + 3, mouseY - 1, water_id, 0, 0);
            w.updateParticle(mouseX - 1, mouseY + 3, water_id, 0, 0);
            w.updateParticle(mouseX - 2, mouseY + 2, water_id, 0, 0);
            w.updateParticle(mouseX - 3, mouseY + 1, water_id, 0, 0);
        }
        e->update(handle_Input, copyTex);
        int ran = frame_number % 2;
        for (int y = height - 1; y > 0; y--)
        {
            for (int x = ran ? 0 : width - 1; ran ? x < width : x > 0; ran ? ++x : --x)
            {
                uint32_t id = w.readParticle(x, y).id;

                switch (id)
                {
                case sand_id:
                    updatesand(x, y, w);

                case water_id:
                    updatewater(x, y, w);
                }
            }
        }
        frame_number++;
    }

    delete e;
    return 0;
}
