#include "mesh.h"
#include "util.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <glad/glad.h>

#define TEX_AT(s, idx, prop) { \
    memset(s, 0, sizeof(s)); \
    sprintf(s, "material.%s%zu", prop, idx); \
}

struct Mesh *mesh_alloc(Vertex *verts, size_t nverts, unsigned int *indices, size_t nindices, struct Texture **textures, size_t ntextures)
{
    struct Mesh *m = malloc(sizeof(struct Mesh));

    glm_vec3_copy((vec3){ 0.f, 0.f, 0.f }, m->pos);
    glm_vec3_copy((vec3){ 0.f, 0.f, 0.f }, m->rot);

    m->verts = verts;
    m->nverts = nverts;

    m->indices = indices;
    m->nindices = nindices;

    m->textures = textures;
    m->ntextures = ntextures;

    glGenVertexArrays(1, &m->vao);
    glGenBuffers(1, &m->vb);
    glGenBuffers(1, &m->ib);

    glBindVertexArray(m->vao);

    glBindBuffer(GL_ARRAY_BUFFER, m->vb);
    glBufferData(GL_ARRAY_BUFFER, nverts * sizeof(Vertex), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nindices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // verts
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // norms
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));
    glEnableVertexAttribArray(1);

    // tex
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
    glEnableVertexAttribArray(2);

    return m;
}


void mesh_free(struct Mesh *m)
{
    free(m->textures);

    free(m->verts);
    free(m->indices);

    free(m);
}


void mesh_render(struct Mesh *m, RenderInfo *ri, mat4 model)
{
    unsigned int diffuse_n = 1;
    unsigned int specular_n = 1;

    for (size_t i = 0; i < m->ntextures; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        bool is_diffuse = m->textures[i]->type == TEXTURETYPE_DIFFUSE;

        char s[100];
        const char *prop = is_diffuse ? "diffuse" : "specular";
        size_t num = is_diffuse ? diffuse_n++ : specular_n++;

        TEX_AT(s, num, prop);

        shader_int(ri->shader, s, i);
        glBindTexture(GL_TEXTURE_2D, m->textures[i]->id);
    }

    shader_float(ri->shader, "material.shininess", 32.f);

    shader_mat4(ri->shader, "model", model);
    shader_mat4(ri->shader, "view", ri->view);
    shader_mat4(ri->shader, "projection", ri->proj);

    glBindVertexArray(m->vao);
    glDrawElements(GL_TRIANGLES, m->nindices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

