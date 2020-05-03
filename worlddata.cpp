#include "worlddata.h"

world_data::world_data(int width, int height) : _w(width), _h(height), p(new particle[width * height])
{
    for (int y = 0; y < _h; y++)
    {
        for (int x = 0; x < _w; x++)
        {
            particle t;
            t.id = empty_id;
            t.vel = 0;
            t.acc = 0;
            p[x + y * _w] = t;
        }
    }
}