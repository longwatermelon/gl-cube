#include "prog.h"
#include <stdlib.h>


struct Prog *prog_alloc(GLFWwindow *win)
{
    struct Prog *p = malloc(sizeof(struct Prog));
    p->win = win;

    return p;
}


void prog_free(struct Prog *p)
{
    free(p);
}


void prog_mainloop(struct Prog *p)
{
    while (!glfwWindowShouldClose(p->win))
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(p->win);
        glfwPollEvents();
    }
}

