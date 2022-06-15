#include "light.h"
#include "shader.h"
#include <string.h>
#include <glad/glad.h>


Phong phong(vec3 ambient, vec3 diffuse, vec3 specular)
{
    Phong p;

    glm_vec3_dup(ambient, p.ambient);
    glm_vec3_dup(diffuse, p.diffuse);
    glm_vec3_dup(specular, p.specular);

    return p;
}


struct Light *light_alloc(vec3 pos, Phong col)
{
    struct Light *l = malloc(sizeof(struct Light));
    glm_vec3_dup(pos, l->pos);
    l->col = col;

    return l;
}


void light_free(struct Light *l)
{
    free(l);
}


void light_set_props(struct Light *l, unsigned int shader, int idx)
{
    char tmp[100] = { 0 };
    sprintf(tmp, "lights[%d].position", idx);
    shader_vec3(shader, tmp, l->pos);

    memset(tmp, 0, sizeof(tmp));
    sprintf(tmp, "lights[%d].ambient", idx);
    shader_vec3(shader, tmp, l->col.ambient);

    memset(tmp, 0, sizeof(tmp));
    sprintf(tmp, "lights[%d].diffuse", idx);
    shader_vec3(shader, tmp, l->col.diffuse);

    memset(tmp, 0, sizeof(tmp));
    sprintf(tmp, "lights[%d].specular", idx);
    shader_vec3(shader, tmp, l->col.specular);
}


struct Material *mat_alloc(Phong col, float shininess)
{
    struct Material *m = malloc(sizeof(struct Material));
    m->col = col;
    m->shininess = shininess;

    return m;
}


void mat_free(struct Material *m)
{
    free(m);
}


void mat_set_props(struct Material *m, unsigned int shader)
{
    shader_vec3(shader, "material.ambient", m->col.ambient);
    shader_vec3(shader, "material.diffuse", m->col.diffuse);
    shader_vec3(shader, "material.specular", m->col.specular);
    shader_float(shader, "material.shininess", m->shininess);
}

