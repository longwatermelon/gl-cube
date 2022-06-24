#ifndef MODEL_H
#define MODEL_H

#include "util.h"
#include "mesh.h"
#include <limits.h>
#include <assimp/scene.h>

struct Model
{
    vec3 pos, rot;

    struct Mesh **meshes;
    size_t nmeshes;

    char dir[PATH_MAX];

    struct Texture **textures_loaded;
    size_t ntextures_loaded;

    mat4 translation, rotation;
};

struct Model *model_alloc(vec3 pos, vec3 rot, const char *path);
void model_free(struct Model *m);

void model_render(struct Model *m, RenderInfo *ri);

void model_move(struct Model *m, vec3 dir);
void model_rot(struct Model *m, vec3 rot);

void model_process_node(struct Model *m, struct aiNode *node, const struct aiScene *sc);
struct Mesh *model_process_mesh(struct Model *m, struct aiMesh *mesh, const struct aiScene *sc);

struct Texture **model_load_mat_textures(struct Model *m, struct aiMaterial *mat, enum aiTextureType type, int ttype, size_t *ntextures);

#endif

