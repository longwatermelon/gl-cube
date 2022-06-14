#include "camera.h"


struct Camera *cam_alloc(vec3 pos)
{
    struct Camera *c = malloc(sizeof(struct Camera));
    glm_vec3_dup(pos, c->pos);

    return c;
}


void cam_free(struct Camera *c)
{
    free(c);
}

