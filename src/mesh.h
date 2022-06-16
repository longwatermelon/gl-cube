#ifndef MESH_H
#define MESH_H

#include "util.h"
#include "texture.h"
#include <cglm/cglm.h>

typedef struct
{
    vec3 pos, norm;
    vec2 tex_coords;
} Vertex;

struct Mesh
{
    Vertex *verts;
    size_t nverts;

    unsigned int *indices;
    size_t nindices;

    struct Texture **textures;
    size_t ntextures;

    unsigned int vao, vb, ib;
};

// Memory ownership of verts, indices, textures is transferred to mesh
struct Mesh *mesh_alloc(Vertex *verts, size_t nverts, unsigned int *indices, size_t nindices, struct Texture **textures, size_t ntextures);
void mesh_free(struct Mesh *m);

void mesh_render(struct Mesh *m, RenderInfo *ri, mat4 model);

#endif

