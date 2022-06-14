#ifndef UTIL_H
#define UTIL_H

#include "camera.h"
#include <cglm/cglm.h>

typedef struct
{
    unsigned int shader;

    struct Camera *cam;
    mat4 view, proj;
} RenderInfo;

char *util_read_file(const char *path);

#endif

