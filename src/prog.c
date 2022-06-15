#include "prog.h"
#include "cube.h"
#include "light.h"
#include <stdlib.h>


struct Prog *prog_alloc(GLFWwindow *win)
{
    struct Prog *p = malloc(sizeof(struct Prog));
    p->win = win;

    vec3 cam_pos;
    glm_vec3_copy((vec3){ 2.f, 0.f, 0.f }, cam_pos);
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
    glUseProgram(p->shader);
    shader_int(p->shader, "nlights", 2);
    struct Cube *c = cube_alloc((vec3){ 0.f, -1.f, -5.f });

    struct Light *lights[2] = {
        light_alloc((vec3){ 1.f, -1.f, -5.f }, phong(
            (vec3){ .2f, .2f, .2f },
            (vec3){ .5f, .5f, .5f },
            (vec3){ 1.f, 1.f, 1.f }
        )),
        light_alloc((vec3){ 0.f, 3.f, -5.f }, phong(
            (vec3){ .2f, .2f, .2f },
            (vec3){ .5f, .5f, .5f },
            (vec3){ 1.f, 1.f, 1.f }
        ))
    };

    struct Material *mat = mat_alloc(phong(
        (vec3){ 1.f, .5f, .31f },
        (vec3){ 1.f, .5f, .31f },
        (vec3){ .5f, .5f, .5f }
    ), 32.f);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(p->win))
    {
        cube_rot(c, 2.f, (vec3){ 1.f, 0.8f, .5f });
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm_mat4_identity(p->ri.view);
        glm_vec3_negate(p->cam->pos);
        glm_translate(p->ri.view, p->cam->pos);
        glm_vec3_negate(p->cam->pos);

        mat_set_props(mat, p->ri.shader);

        for (size_t i = 0; i < 2; ++i)
            light_set_props(lights[i], p->ri.shader, i);

        cube_render(c, &p->ri);

        glfwSwapBuffers(p->win);
        glfwPollEvents();
    }

    cube_free(c);
}

