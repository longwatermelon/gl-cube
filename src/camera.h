#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

struct Camera
{
    vec3 pos;
};

struct Camera *cam_alloc(vec3 pos);
void cam_free(struct Camera *c);

#endif

