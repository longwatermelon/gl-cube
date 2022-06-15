#ifndef CUBE_H
#define CUBE_H

#include "util.h"
#include <cglm/cglm.h>

struct Cube
{
    vec3 pos;
    unsigned int vao, vb;

    mat4 model;
};

struct Cube *cube_alloc(vec3 pos);
void cube_free(struct Cube *c);

void cube_render(struct Cube *c, RenderInfo *ri);

void cube_move(struct Cube *c, vec3 dir);
void cube_rot(struct Cube *c, float angle, vec3 axis);

#endif

