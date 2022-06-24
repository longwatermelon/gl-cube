#ifndef RENDER_H
#define RENDER_H

#include "camera.h"
#include <cglm/cglm.h>

typedef struct
{
    unsigned int shader;

    mat4 proj, view;
    struct Camera *cam;
} RenderInfo;

#endif

