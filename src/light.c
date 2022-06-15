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
    glm_vec3_dup(pos, l->pos);
    l->col = col;
    l->att = att;

    return l;
}


void light_free(struct Light *l)
{
    free(l);
}


void light_set_props(struct Light *l, unsigned int shader, int idx)
{
    char s[100];
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
}


struct Material *mat_alloc(struct Texture *diff, struct Texture *spec, float shininess)
{
    struct Material *m = malloc(sizeof(struct Material));
    m->diffuse = diff;
    m->specular = spec;

    m->shininess = shininess;

    return m;
}


void mat_free(struct Material *m)
{
    free(m);
}


void mat_set_props(struct Material *m, unsigned int shader)
{
    shader_float(shader, "material.shininess", m->shininess);

    shader_int(shader, "material.diffuse", m->diffuse->tex_slot);
    tex_bind(m->diffuse);

    shader_int(shader, "material.specular", m->specular->tex_slot);
    tex_bind(m->specular);
}

