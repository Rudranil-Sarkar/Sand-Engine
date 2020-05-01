#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <stdint.h>

struct particle
{
    uint32_t id;
    int vel;
    int acc;
};

enum par_id
{
    sand_id = 0xFFFFDB58
};

#endif