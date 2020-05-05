#include "./engine/engine.h"
#include "./particles/particles.h"
#include "worlddata.h"
#include <iostream>
#include <cstdlib>

Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;
double dt = 0;

int width = 640;
int height = 480;

world_data w(width, height);

bool leftMouseButtonDown, change_p;
int mouseX;
int mouseY;

int random_value(int low, int high)
{
    int range = (high - low) + 1;
    return range * (rand() / (RAND_MAX + 1.));
}
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
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_a)
                change_p = true;
            break;
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

    if (w.readParticle(x, y).has_updated)
    {
        w.readParticle(x, y).has_updated = false;
        return;
    }

    particle this_particle = w.readParticle(x, y);
    particle below = w.readParticle(x, y + 1);

    this_particle.acc = e->gravity;
    this_particle.vel += this_particle.acc;
    if (below.id == empty_id)
    {
        int prevx = x;
        int prevy = y;
        for (int i = 0; i <= this_particle.vel; i++)
        {
            if (x + i >= width || y + i >= height)
                break;

            w.updateParticle(prevx, prevy, empty_id, 0, 0);
            w.updateParticle(x, y + i, sand_id, this_particle.vel, this_particle.acc);
            prevx = x;
            prevy = y + i;
            if (w.readParticle(x, y + i + 1).id != empty_id)
            {
                w.readParticle(prevx, prevy).vel /= 2;
                break;
            }
        }
        w.readParticle(prevx, prevy).has_updated = true;
        return;
    }

    particle below_left = w.readParticle(x - 1, y + 1);

    if (below_left.id == empty_id)
    {
        int prevx = x;
        int prevy = y;
        for (int i = 0; i <= this_particle.vel; i++)
        {
            if (x + i >= width || y + i >= height || x - i <= 0)
                break;

            w.updateParticle(prevx, prevy, empty_id, 0, 0);
            w.updateParticle(x - i, y + i, sand_id, this_particle.vel, this_particle.acc);
            prevx = x - i;
            prevy = y + i;
            if (w.readParticle(x - i - 1, y + i + 1).id != empty_id)
            {
                w.readParticle(prevx, prevy).vel /= 2;
                break;
            }
        }
        w.readParticle(prevx, prevy).has_updated = true;
        return;
    }

    particle below_right = w.readParticle(x + 1, y + 1);

    if (below_right.id == empty_id)
    {
        int prevx = x;
        int prevy = y;
        for (int i = 0; i <= this_particle.vel; i++)
        {
            if (x + i >= width || y + i >= height || x - i <= 0)
                break;

            w.updateParticle(prevx, prevy, empty_id, 0, 0);
            w.updateParticle(x + i, y + i, sand_id, this_particle.vel, this_particle.acc);
            prevx = x + i;
            prevy = y + i;
            if (w.readParticle(x + i + 1, y + i + 1).id != empty_id)
            {
                w.readParticle(prevx, prevy).vel /= 2;
                break;
            }
        }
        w.readParticle(prevx, prevy).has_updated = true;
        return;
    }
}

void updatewater(int x, int y, world_data &w)
{
#define water_flow 5
    if (x >= width - 1 || y >= height - 1)
        return;

    particle this_particle = w.readParticle(x, y);
    if (w.readParticle(x, y).has_updated)
    {
        w.readParticle(x, y).has_updated = false;
        return;
    }
    particle below = w.readParticle(x, y + 1);

    this_particle.acc = e->gravity;
    this_particle.vel += this_particle.acc;

    if (below.id == empty_id)
    {
        int prevx = x;
        int prevy = y;

        for (int i = 0; i <= this_particle.vel; i++)
        {
            if (x + i >= width - 1 || y + i >= height - 1)
                break;
            w.updateParticle(prevx, prevy, empty_id, 0, 0);
            w.updateParticle(x, y + i, water_id, this_particle.vel, this_particle.acc);
            w.readParticle(prevx, prevy).has_updated = false;
            prevx = x;
            prevy = y + i;
            if (w.readParticle(x, y + i + 1).id != empty_id)
            { 
		this_particle.vel /= 2;
                break;
            }
        }
        w.readParticle(x, y).has_updated = false;
        w.readParticle(prevx, prevy).has_updated = true;
        return;
    }
    particle below_left = w.readParticle(x - 1, y + 1);

    if (below_left.id == empty_id)
    {
        int prevx = x;
        int prevy = y;
        for (int i = 0; i <= this_particle.vel; i++)
        {

            if (x + i >= width - 1 || y + i >= height - 1 || x - i <= 0)
                break;

            w.updateParticle(prevx, prevy, empty_id, 0, 0);
            w.updateParticle(x - i, y + i, water_id, this_particle.vel, this_particle.acc);
            w.readParticle(prevx, prevy).has_updated = false;
            prevx = x - i;
            prevy = y + i;
            if (w.readParticle(x - i - 1, y + i + 1).id != empty_id)
            {
		this_particle.vel /= 2;
                break;
            }
        }
        w.readParticle(x, y).has_updated = false;
        w.readParticle(prevx, prevy).has_updated = true;
        return;
    }

    particle below_right = w.readParticle(x + 1, y + 1);

    if (below_right.id == empty_id)
    {
        int prevx = x;
        int prevy = y;
        for (int i = 0; i <= this_particle.vel; i++)
        {

            if (x + i >= width - 1 || y + i >= height - 1 || x - i <= 0)
                break;

            w.updateParticle(prevx, prevy, empty_id, 0, 0);
            w.updateParticle(x + i, y + i, water_id, this_particle.vel, this_particle.acc);
            w.readParticle(prevx, prevy).has_updated = false;
            prevx = x + i;
            prevy = y + i;
            if (w.readParticle(x + i + 1, y + i + 1).id != empty_id)
            {
		this_particle.vel /= 2;
                break;
            }
        }
        w.readParticle(x, y).has_updated = false;
        w.readParticle(prevx, prevy).has_updated = true;
        return;
    }

    int ran = random_value(0, 1);
    int r = ran ? water_flow : -water_flow;

    particle left = w.readParticle(x - 1, y);

    if (left.id == empty_id)
    {
        int prevx = x;
        int prevy = y;
        for (int i = 0; i <= water_flow; i++)
        {

            if (x + i >= width - 1 || x - i <= 0)
                break;

            w.updateParticle(prevx, prevy, empty_id);
            w.updateParticle(x - i, y, water_id, this_particle.vel / 2, 0);
            w.readParticle(prevx, prevy).has_updated = false;
            prevx = x - i;
            prevy = y;
            if (w.readParticle(x - i - 1, y).id != empty_id)
            {
                break;
            }
        }
        w.readParticle(x, y).has_updated = false;
        w.readParticle(prevx, prevy).has_updated = true;
        return;
    }
    particle right = w.readParticle(x + 1, y);

    if (right.id == empty_id)
    {
        int prevx = x;
        int prevy = y;
        for (int i = 0; i <= water_flow; i++)
        {
            if (x + i >= width - 1 || x - i <= 0)
                break;

            w.updateParticle(prevx, prevy, empty_id);
            w.updateParticle(x + i, y, water_id, this_particle.vel / 2, 0);
            w.readParticle(prevx, prevy).has_updated = false;
            prevx = x + i;
            prevy = y;
            if (w.readParticle(x + i + 1, y).id != empty_id)
            {
                w.readParticle(prevx, prevy).vel = 0;
                break;
            }
        }
        w.readParticle(x, y).has_updated = false;
        w.readParticle(prevx, prevy).has_updated = true;
        return;
    }
}

int main(int argc, char **argv)
{
    int frame_number = 0;
    uint32_t selected_particle = wood_id;
    while (e->running)
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        dt = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());

        if (change_p)
        {
            std::cout << "\n>> ";
            int c;
            std::cin >> c;

            switch (c)
            {
            case 1:
                selected_particle = wood_id;
                break;

            case 2:
                selected_particle = sand_id;
                break;
            case 3:
                selected_particle = water_id;
                break;
            case 4:
                selected_particle = empty_id;
                break;

            default:
                break;
            }
            change_p = false;
        }

        if (leftMouseButtonDown)
        {
            if (selected_particle == wood_id || selected_particle == empty_id)
            {
                float radius = 10.f;
                for (int i = -radius; i < radius; ++i)
                {
                    for (int j = -radius; j < radius; ++j)
                    {
                        int rx = mouseX + j;
                        int ry = mouseY + i;

                        float dist = sqrt(((ry - mouseY) * (ry - mouseY)) + ((rx - mouseX) * (rx - mouseX)));
                        if (dist <= radius)
                            w.updateParticle(rx, ry, selected_particle);
                    }
                }
            }
            int r_amt = random_value(10, 100);

            for (int i = 0; i < r_amt; ++i)
            {
                float ran = (float)random_value(0, 100) / 100.f;
                int r = 10 * sqrt(ran);
                float theta = (float)random_value(0, 100) / 100.f * 2.f * 3.14f;
                float rx = cos((float)theta) * r;
                float ry = sin((float)theta) * r;
                int mpx = mouseX + rx;
                int mpy = mouseY + ry;
                if (w.readParticle(mpx, mpy).id == empty_id)
                {
                    particle p = {0};
                    p.id = selected_particle;
                    w.updateParticle(mpx, mpy, p.id);
                }
            }
        }
        e->update(handle_Input, copyTex);
        int ran = frame_number % 2;
        for (int y = 0; y < height - 1; y++)
        {
            for (int x = ran ? 0 : width - 1; ran ? x < width : x > 0; ran ? ++x : --x)
            {
                uint32_t id = w.readParticle(x, y).id;

                switch (id)
                {
                case sand_id:
                    updatesand(x, y, w);
                    break;

                case water_id:
                    updatewater(x, y, w);
                    break;
                case wood_id:
                    break;
                }
            }
        }
        copyTex();
        frame_number++;
    }

    delete e;
    return 0;
}
