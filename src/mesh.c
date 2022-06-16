#include "mesh.h"
#include "util.h"
#include <string.h>
#include <stdlib.h>
#include <glad/glad.h>

#define TEX_AT(s, idx, prop) { \
    memset(s, 0, sizeof(s)); \
    sprintf(s, "%s%zu", prop, idx); \
}

struct Mesh *mesh_alloc(Vertex *verts, size_t nverts, unsigned int *indices, size_t nindices, struct Texture **textures, size_t ntextures)
{
    struct Mesh *m = malloc(sizeof(struct Mesh));

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
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(vec3));

    // tex
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec3) * 2));

    glm_mat4_identity(m->model);

    return m;
}


void mesh_free(struct Mesh *m)
{
    free(m->verts);
    free(m->indices);

    for (size_t i = 0; i < m->ntextures; ++i)
        tex_free(m->textures[i]);

    free(m->textures);

    free(m);
}


void mesh_render(struct Mesh *m, RenderInfo *ri)
{
    glUseProgram(ri->shader);

    unsigned int diffuse_n = 1;
    unsigned int specular_n = 1;

    for (size_t i = 0; i < m->ntextures; ++i)
    {
        bool is_diffuse = m->textures[i]->type == TEXTURETYPE_DIFFUSE;

        char s[100];
        const char *prop = is_diffuse ? "diffuse" : "specular";
        size_t num = is_diffuse ? diffuse_n++ : specular_n++;

        TEX_AT(s, num, prop);

        shader_float(ri->shader, s, i);
        tex_bind(m->textures[i], GL_TEXTURE0 + i);
    }

    shader_mat4(ri->shader, "model", m->model);
    shader_mat4(ri->shader, "view", ri->view);
    shader_mat4(ri->shader, "projection", ri->proj);

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(m->vao);
    glDrawElements(GL_TRIANGLES, m->nindices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
