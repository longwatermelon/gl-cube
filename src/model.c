#include "model.h"
#include "util.h"
#include <unistd.h>
#include <libgen.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>


struct Model *model_alloc(vec3 pos, vec3 rot, const char *path)
{
    struct Model *m = malloc(sizeof(struct Model));
    const struct aiScene *scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        fprintf(stderr, "[model_alloc] Assimp error when loading model.\n");
        exit(EXIT_FAILURE);
    }

    memset(m->dir, 0, sizeof(m->dir));
    char *pc = strdup(path);
    char *dir = dirname(pc);
    sprintf(m->dir, "%s", dir);

    free(pc);

    m->meshes = 0;
    m->nmeshes = 0;

    m->textures_loaded = 0;
    m->ntextures_loaded = 0;

    model_process_node(m, scene->mRootNode, scene);

    glm_mat4_identity(m->translation);
    glm_mat4_identity(m->rotation);

    glm_vec3_copy((vec3){ 0.f, 0.f, 0.f }, m->pos);
    glm_vec3_copy((vec3){ 0.f, 0.f, 0.f }, m->rot);

    model_move(m, pos);
    model_rot(m, rot);

    return m;
}


void model_free(struct Model *m)
{
    for (size_t i = 0; i < m->nmeshes; ++i)
        mesh_free(m->meshes[i]);

    free(m->meshes);

    for (size_t i = 0; i < m->ntextures_loaded; ++i)
        tex_free(m->textures_loaded[i]);

    free(m->textures_loaded);
    free(m);
}


void model_render(struct Model *m, RenderInfo *ri)
{
    mat4 model;
    glm_mat4_mul(m->translation, m->rotation, model);

    for (size_t i = 0; i < m->nmeshes; ++i)
        mesh_render(m->meshes[i], ri, model);
}


void model_move(struct Model *m, vec3 dir)
{
    glm_translate(m->translation, dir);
    glm_vec3_add(m->pos, dir, m->pos);
}


void model_rot(struct Model *m, vec3 rot)
{
    glm_vec3_add(m->rot, rot, m->rot);

    vec4 quat;
    util_quat_from_rot(m->rot, quat);

    glm_mat4_identity(m->rotation);
    glm_quat_rotate(m->rotation, quat, m->rotation);
}


void model_process_node(struct Model *m, struct aiNode *node, const struct aiScene *sc)
{
    for (size_t i = 0; i < node->mNumMeshes; ++i)
    {
        struct aiMesh *mesh = sc->mMeshes[node->mMeshes[i]];
        m->meshes = realloc(m->meshes, sizeof(struct Mesh*) * ++m->nmeshes);
        m->meshes[m->nmeshes - 1] = model_process_mesh(m, mesh, sc);
    }

    for (size_t i = 0; i < node->mNumChildren; ++i)
    {
        model_process_node(m, node->mChildren[i], sc);
    }
}


struct Mesh *model_process_mesh(struct Model *m, struct aiMesh *mesh, const struct aiScene *sc)
{
    Vertex *vertices = 0;
    size_t nverts = 0;

    unsigned int *indices = 0;
    size_t nindices = 0;

    struct Texture **textures = 0;
    size_t ntextures = 0;

    vertices = malloc(sizeof(Vertex) * mesh->mNumVertices);
    nverts = mesh->mNumVertices;

    // verts
    for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex v;
        v.pos[0] = mesh->mVertices[i].x;
        v.pos[1] = mesh->mVertices[i].y;
        v.pos[2] = mesh->mVertices[i].z;

        v.norm[0] = mesh->mNormals[i].x;
        v.norm[1] = mesh->mNormals[i].y;
        v.norm[2] = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0])
        {
            v.tex_coords[0] = mesh->mTextureCoords[0][i].x;
            v.tex_coords[1] = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            glm_vec2_copy((vec2){ 0.f, 0.f }, v.tex_coords);
        }

        vertices[i] = v;
    }

    // indices
    for (size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        struct aiFace face = mesh->mFaces[i];

        for (size_t j = 0; j < face.mNumIndices; ++j)
        {
            indices = realloc(indices, sizeof(unsigned int) * ++nindices);
            indices[nindices - 1] = face.mIndices[j];
        }
    }

    // material
    struct aiMaterial *mat = sc->mMaterials[mesh->mMaterialIndex];

    size_t ndiff;
    struct Texture **diffuse_maps = model_load_mat_textures(m, mat, aiTextureType_DIFFUSE, TEXTURETYPE_DIFFUSE, &ndiff);

    size_t nspec;
    struct Texture **specular_maps = model_load_mat_textures(m, mat, aiTextureType_SPECULAR, TEXTURETYPE_SPECULAR, &nspec);

    ntextures = ndiff + nspec;
    textures = malloc(sizeof(struct Texture*) * ntextures);
    memcpy(textures, diffuse_maps, ndiff * sizeof(struct Texture*));
    memcpy(&textures[ndiff], specular_maps, nspec * sizeof(struct Texture*));

    free(diffuse_maps);
    free(specular_maps);

    return mesh_alloc(vertices, nverts, indices, nindices, textures, ntextures);
}


struct Texture **model_load_mat_textures(struct Model *m, struct aiMaterial *mat, enum aiTextureType type, int ttype, size_t *ntextures)
{
    *ntextures = aiGetMaterialTextureCount(mat, type);
    struct Texture **textures = malloc(sizeof(struct Texture*) * *ntextures);

    for (size_t i = 0; i < *ntextures; ++i)
    {
        struct aiString s;
        aiGetMaterialTexture(mat, type, i, &s, 0, 0, 0, 0, 0, 0);

        char *snew = calloc(sizeof(char), strlen(m->dir) + strlen(s.data) + 2);
        sprintf(snew, "%s/%s", m->dir, s.data);

        bool skip = false;

        for (size_t j = 0; j < m->ntextures_loaded; ++j)
        {
            if (strcmp(m->textures_loaded[j]->path, snew) == 0)
            {
                textures[i] = m->textures_loaded[j];
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            textures[i] = tex_alloc(snew, ttype);

            m->textures_loaded = realloc(m->textures_loaded, sizeof(struct Texture*) * ++m->ntextures_loaded);
            m->textures_loaded[m->ntextures_loaded - 1] = textures[i];
        }
    }

    return textures;
}

