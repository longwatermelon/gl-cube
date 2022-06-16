#ifndef RENDER_H
#define RENDER_H

#include "camera.h"
#include <cglm/cglm.h>

typedef struct
{
    unsigned int shader;

    struct Camera *cam;
    mat4 view, proj;
} RenderInfo;

#endif

