#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <stdint.h>

struct particle
{
    uint32_t id;
    int vel;
    int acc;
    bool has_updated = false;
};

enum par_id
{
    empty_id = 0xFF282828,
    sand_id = 0xFFFFDB58,
    water_id = 0xFF2e6598,
    wood_id = 0xFFA52A2A
};

#endif