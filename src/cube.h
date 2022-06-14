#ifndef CUBE_H
#define CUBE_H

#include "util.h"
#include <cglm/cglm.h>

struct Cube
{
    vec3 pos, rot;
    unsigned int vao, vb;

    mat4 model;
};

struct Cube *cube_alloc(vec3 pos, vec3 rot);
void cube_free(struct Cube *c);

void cube_render(struct Cube *c, RenderInfo *ri);

#endif

