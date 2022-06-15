#ifndef LIGHT_H
#define LIGHT_H

#include <cglm/cglm.h>

typedef struct
{
    vec3 ambient, diffuse, specular;
} Phong;

Phong phong(vec3 ambient, vec3 diffuse, vec3 specular);

struct Light
{
    vec3 pos;
    Phong col;
};

struct Light *light_alloc(vec3 pos, Phong col);
void light_free(struct Light *l);

void light_set_props(struct Light *l, unsigned int shader);

struct Material
{
    Phong col;
    float shininess;
};

struct Material *mat_alloc(Phong col, float shininess);
void mat_free(struct Material *m);

void mat_set_props(struct Material *m, unsigned int shader);

#endif

