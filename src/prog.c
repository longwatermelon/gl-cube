#include "prog.h"
#include "light.h"
#include "model.h"
#include <stb/stb_image.h>
#include <stdlib.h>


struct Prog *prog_alloc(GLFWwindow *win)
{
    struct Prog *p = malloc(sizeof(struct Prog));
    p->win = win;

    vec3 cam_pos;
    glm_vec3_copy((vec3){ 0.f, 0.f, 5.f }, cam_pos);
    p->cam = cam_alloc(cam_pos, (vec3){ -90.f, 0.f, 0.f });

    p->ri.shader = shader_create("shaders/basic_v.glsl", "shaders/basic_f.glsl");

    p->ri.cam = p->cam;
    glm_mat4_identity(p->ri.proj);
    glm_mat4_identity(p->ri.view);

    glm_perspective(glm_rad(45.f), 800.f / 600.f, .1f, 100.f, p->ri.proj);

    stbi_set_flip_vertically_on_load(true);

    return p;
}


void prog_free(struct Prog *p)
{
    cam_free(p->cam);

    glDeleteShader(p->ri.shader);
    free(p);
}


void prog_mainloop(struct Prog *p)
{
    glUseProgram(p->ri.shader);

    struct Model *m = model_alloc((vec3){ 0.f, 0.f, 0.f }, "res/knife/300 sword/OBJ/sword.obj");
    printf("Finished processing model\n");

    struct Light *lights[2] = {
        light_spotlight(light_alloc((vec3){ 3.f, -1.f, -5.f }, phong(
            (vec3){ .2f, .2f, .2f },
            (vec3){ .5f, .5f, .5f },
            (vec3){ 1.f, 1.f, 1.f }
        ), (Attenuation){
            .constant = 1.f,
            .linear = .09f,
            .quadratic = .032f
        }), (vec3){ 0.f, 0.f, -1.f }, cosf(glm_rad(14.5f)), cosf(glm_rad(20.5f))),
        light_alloc((vec3){ 0.f, 3.f, 0.f }, phong(
            (vec3){ .2f, .2f, .2f },
            (vec3){ .5f, .5f, .5f },
            (vec3){ 1.f, 1.f, 1.f }
        ), (Attenuation){
            .constant = 1.f,
            .linear = .09f,
            .quadratic = .032f
        })
    };

    glEnable(GL_DEPTH_TEST);

    glfwSetCursorPos(p->win, 400.f, 300.f);
    glfwSetInputMode(p->win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    double prev_mx, prev_my;
    glfwGetCursorPos(p->win, &prev_mx, &prev_my);

    while (!glfwWindowShouldClose(p->win))
    {
        double mx, my;
        glfwGetCursorPos(p->win, &mx, &my);

        cam_rot(p->cam, (vec3){ (mx - prev_mx) / 5.f, -(my - prev_my) / 5.f, 0.f });
        prev_mx = mx;
        prev_my = my;

        prog_events(p);

        glm_vec3_copy(p->cam->pos, lights[0]->pos);

        vec3 diff;
        glm_vec3_sub(p->cam->front, lights[0]->spotlight_dir, diff);
        glm_vec3_scale(diff, 1.f / 5.f, diff);

        glm_vec3_add(lights[0]->spotlight_dir, diff, lights[0]->spotlight_dir);
        model_rot(m, glm_rad(2.f), (vec3){ 1.f, .5f, .8f });

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm_look(p->cam->pos, p->cam->front, p->cam->up, p->ri.view);

        for (size_t i = 0; i < 2; ++i)
            light_set_props(lights[i], p->ri.shader, i);

        model_render(m, &p->ri);

        glfwSwapBuffers(p->win);
        glfwPollEvents();
    }

    model_free(m);
}


void prog_events(struct Prog *p)
{
    float move = .05f;
    float rot = 2.f;

    vec3 front;
    glm_vec3_scale(p->cam->front, move, front);
    front[1] = 0.f;

    vec3 right;
    glm_vec3_scale(p->cam->right, move, right);
    right[1] = 0.f;

    if (glfwGetKey(p->win, GLFW_KEY_W) == GLFW_PRESS) glm_vec3_add(p->cam->pos, front, p->cam->pos);
    if (glfwGetKey(p->win, GLFW_KEY_S) == GLFW_PRESS) glm_vec3_sub(p->cam->pos, front, p->cam->pos);
    if (glfwGetKey(p->win, GLFW_KEY_A) == GLFW_PRESS) glm_vec3_sub(p->cam->pos, right, p->cam->pos);
    if (glfwGetKey(p->win, GLFW_KEY_D) == GLFW_PRESS) glm_vec3_add(p->cam->pos, right, p->cam->pos);

    if (glfwGetKey(p->win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) p->cam->pos[1] -= move;
    if (glfwGetKey(p->win, GLFW_KEY_SPACE) == GLFW_PRESS) p->cam->pos[1] += move;

    if (glfwGetKey(p->win, GLFW_KEY_LEFT) == GLFW_PRESS)
        cam_rot(p->cam, (vec3){ -rot, 0.f, 0.f });

    if (glfwGetKey(p->win, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cam_rot(p->cam, (vec3){ rot, 0.f, 0.f });

    if (glfwGetKey(p->win, GLFW_KEY_UP) == GLFW_PRESS)
        cam_rot(p->cam, (vec3){ 0.f, rot, 0.f });

    if (glfwGetKey(p->win, GLFW_KEY_DOWN) == GLFW_PRESS)
        cam_rot(p->cam, (vec3){ 0.f, -rot, 0.f });
}

