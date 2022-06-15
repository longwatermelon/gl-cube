#ifndef TEXTURE_H
#define TEXTURE_H

#include "shader.h"

struct Texture
{
    unsigned int id, tex_slot;
};

struct Texture *tex_alloc(const char *path, unsigned int slot, unsigned int shader);
void tex_free(struct Texture *t);

void tex_bind(struct Texture *t);

#endif

