#ifndef PROG_H
#define PROG_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Prog
{
    GLFWwindow *win;
};

struct Prog *prog_alloc(GLFWwindow *win);
void prog_free(struct Prog *p);

void prog_mainloop(struct Prog *p);

#endif

