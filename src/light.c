#include "light.h"
#include "shader.h"
#include <string.h>
#include <glad/glad.h>

#define LIGHT_AT(s, idx, prop) { \
    memset(s, 0, sizeof(s)); \
    sprintf(s, "lights[%d]." prop, idx); \
}


Phong phong(vec3 ambient, vec3 diffuse, vec3 specular)
{
    Phong p;

    glm_vec3_dup(ambient, p.ambient);
    glm_vec3_dup(diffuse, p.diffuse);
    glm_vec3_dup(specular, p.specular);

    return p;
}


struct Light *light_alloc(vec3 pos, Phong col, Attenuation att)
{
    struct Light *l = malloc(sizeof(struct Light));
    l->type = LIGHT_POINT;
    glm_vec3_dup(pos, l->pos);
    l->col = col;
    l->att = att;

    return l;
}


void light_free(struct Light *l)
{
    free(l);
}


struct Light *light_spotlight(struct Light *l, vec3 dir, float cutoff, float outer_cutoff)
{
    l->type = LIGHT_SPOTLIGHT;

    glm_vec3_dup(dir, l->spotlight_dir);
    l->spotlight_cutoff = cutoff;
    l->spotlight_outer_cutoff = outer_cutoff;

    return l;
}


void light_set_props(struct Light *l, unsigned int shader, int idx)
{
    char s[100];

    // base parameters
    LIGHT_AT(s, idx, "type");
    shader_int(shader, s, l->type);

    LIGHT_AT(s, idx, "position");
    shader_vec3(shader, s, l->pos);

    LIGHT_AT(s, idx, "ambient");
    shader_vec3(shader, s, l->col.ambient);

    LIGHT_AT(s, idx, "diffuse");
    shader_vec3(shader, s, l->col.diffuse);

    LIGHT_AT(s, idx, "specular");
    shader_vec3(shader, s, l->col.specular);

    LIGHT_AT(s, idx, "constant");
    shader_float(shader, s, l->att.constant);

    LIGHT_AT(s, idx, "linear");
    shader_float(shader, s, l->att.linear);

    LIGHT_AT(s, idx, "quadratic");
    shader_float(shader, s, l->att.quadratic);

    // spotlight
    if (l->type == LIGHT_SPOTLIGHT)
    {
        LIGHT_AT(s, idx, "spotlight_dir");
        shader_vec3(shader, s, l->spotlight_dir);

        LIGHT_AT(s, idx, "spotlight_cutoff");
        shader_float(shader, s, l->spotlight_cutoff);

        LIGHT_AT(s, idx, "spotlight_outer_cutoff");
        shader_float(shader, s, l->spotlight_outer_cutoff);
    }
}

