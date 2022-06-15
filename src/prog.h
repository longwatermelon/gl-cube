#ifndef PROG_H
#define PROG_H

#include "util.h"
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Prog
{
    GLFWwindow *win;

    RenderInfo ri;

    struct Camera *cam;
    unsigned int shader;
};

struct Prog *prog_alloc(GLFWwindow *win);
void prog_free(struct Prog *p);

void prog_mainloop(struct Prog *p);

void prog_events(struct Prog *p);

#endif

