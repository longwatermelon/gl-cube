#include "camera.h"


struct Camera *cam_alloc(vec3 pos, vec3 rot)
{
    struct Camera *c = malloc(sizeof(struct Camera));
    glm_vec3_dup(pos, c->pos);
    cam_rot(c, rot);

    return c;
}


void cam_free(struct Camera *c)
{
    free(c);
}


void cam_rot(struct Camera *c, vec3 rot)
{
    glm_vec3_add(c->rot, rot, c->rot);
    cam_update_vectors(c);
}


void cam_update_vectors(struct Camera *c)
{
    vec3 front = {
        cosf(glm_rad(c->rot[0])) * cosf(glm_rad(c->rot[1])),
        sinf(glm_rad(c->rot[1])),
        sinf(glm_rad(c->rot[0])) * cosf(glm_rad(c->rot[1]))
    };

    vec3 right, up;
    glm_cross(front, (vec3){ 0.f, 1.f, 0.f }, right);
    glm_cross(right, front, up);

    glm_vec3_normalize_to(front, c->front);
    glm_vec3_normalize_to(up, c->up);
    glm_vec3_normalize_to(right, c->right);
}

