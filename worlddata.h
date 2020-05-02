#ifndef __WORLD_DATA_H_
#define __WORLD_DATA_H_

#include "./particles/particles.h"
#include <stdint.h>

class world_data
{
private:
    int _w, _h;

public:
    particle *p;
    world_data(int width, int height);
    void updateParticle(int x, int y, uint32_t id, int vel = 0, int acc = 0, int flowrate = 0)
    {
        particle t;
        t.id = id;
        t.vel = vel;
        t.acc = acc;
        t.flowrate = flowrate;
        p[x + y * _w] = t;
    }

    particle &readParticle(int x, int y)
    {
        return p[x + y * _w];
    }

    ~world_data() { delete[] p; }
};

#endif