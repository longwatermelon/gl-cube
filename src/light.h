#ifndef LIGHT_H
#define LIGHT_H

#include "texture.h"
#include <cglm/cglm.h>

typedef struct
{
    vec3 ambient, diffuse, specular;
} Phong;

Phong phong(vec3 ambient, vec3 diffuse, vec3 specular);

typedef struct
{
    float constant, linear, quadratic;
} Attenuation;

struct Light
{
    vec3 pos;
    Phong col;
    Attenuation att;
};

struct Light *light_alloc(vec3 pos, Phong col, Attenuation att);
void light_free(struct Light *l);

void light_set_props(struct Light *l, unsigned int shader, int idx);

struct Material
{
    struct Texture *diffuse, *specular;
    float shininess;
};

struct Material *mat_alloc(struct Texture *diff, struct Texture *spec, float shininess);
void mat_free(struct Material *m);

void mat_set_props(struct Material *m, unsigned int shader);

#endif

