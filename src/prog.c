#include "prog.h"
#include "cube.h"
#include <stdlib.h>


struct Prog *prog_alloc(GLFWwindow *win)
{
    struct Prog *p = malloc(sizeof(struct Prog));
    p->win = win;

    vec3 cam_pos;
    glm_vec3_copy((vec3){ 0.f, 0.f, 3.f }, cam_pos);
    p->cam = cam_alloc(cam_pos);

    p->shader = shader_create("shaders/basic_v.glsl", "shaders/basic_f.glsl");
    p->ri.shader = p->shader;

    p->ri.cam = p->cam;
    glm_mat4_identity(p->ri.proj);
    glm_mat4_identity(p->ri.view);

    glm_perspective(glm_rad(45.f), 800.f / 600.f, .1f, 100.f, p->ri.proj);

    return p;
}


void prog_free(struct Prog *p)
{
    cam_free(p->cam);

    glDeleteShader(p->shader);
    free(p);
}


void prog_mainloop(struct Prog *p)
{
    struct Cube *c = cube_alloc((vec3){ 0.f, 0.f, 0.f }, (vec3){ 0.f, 0.f, 0.f });

    while (!glfwWindowShouldClose(p->win))
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm_mat4_identity(p->ri.view);

        glm_vec3_negate(p->cam->pos);
        glm_translate(p->ri.view, p->cam->pos);
        glm_vec3_negate(p->cam->pos);

        cube_render(c, &p->ri);

        glfwSwapBuffers(p->win);
        glfwPollEvents();
    }

    cube_free(c);
}

