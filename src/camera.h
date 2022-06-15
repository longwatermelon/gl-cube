#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

struct Camera
{
    vec3 pos, rot;

    vec3 front, up, right;
};

struct Camera *cam_alloc(vec3 pos, vec3 rot);
void cam_free(struct Camera *c);

void cam_rot(struct Camera *c, vec3 rot);
void cam_update_vectors(struct Camera *c);

#endif

