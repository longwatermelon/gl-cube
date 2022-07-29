#include "prog.h"
#include "util.h"
#include <stdlib.h>


struct Prog *prog_alloc(GLFWwindow *win)
{
    struct Prog *p = malloc(sizeof(struct Prog));
    p->running = true;
    p->focused = true;
    p->win = win;

    p->cam = cam_alloc((vec3){ 0.f, 0.f, 0.f }, (vec3){ 0.f, 0.f, 0.f });

    p->ri = ri_alloc();
    ri_add_shader(p->ri, SHADER_BASIC, "shaders/basic_v.glsl", "shaders/basic_f.glsl");

    p->ri->cam = p->cam;

    return p;
}


void prog_free(struct Prog *p)
{
    cam_free(p->cam);
    ri_free(p->ri);
    free(p);
}


void prog_mainloop(struct Prog *p)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glfwSetCursorPos(p->win, SCRW / 2.f, SCRH / 2.f);
    glfwSetInputMode(p->win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwGetCursorPos(p->win, &p->prev_mx, &p->prev_my);

    float verts[] = {
        2.f, 0.f, 0.f,
        2.f, -1.f, 0.f,
        2.f, -1.f, 1.f
    };

    unsigned int vao, vb;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (p->running)
    {
        if (p->focused)
        {
            double mx, my;
            glfwGetCursorPos(p->win, &mx, &my);

            cam_rot(p->cam, (vec3){ 0.f, -(my - p->prev_my) / 100.f, -(mx - p->prev_mx) / 100.f });
            p->prev_mx = mx;
            p->prev_my = my;
        }

        prog_events(p);

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ri_use_shader(p->ri, SHADER_BASIC);

        cam_set_props(p->cam, p->ri->shader);
        cam_view_mat(p->cam, p->ri->view);

        shader_mat4(p->ri->shader, "view", p->ri->view);
        shader_mat4(p->ri->shader, "projection", p->ri->proj);

        mat4 model;
        glm_mat4_identity(model);
        shader_mat4(p->ri->shader, "model", model);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(p->win);
        glfwPollEvents();
    }
}


void prog_events(struct Prog *p)
{
    if (glfwWindowShouldClose(p->win)) p->running = false;

    if (p->focused && glfwGetKey(p->win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetInputMode(p->win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        p->focused = false;
    }

    if (!p->focused && glfwGetMouseButton(p->win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glfwGetCursorPos(p->win, &p->prev_mx, &p->prev_my);
        glfwSetInputMode(p->win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        p->focused = true;
    }

    float move = .05f;

    vec3 angle;
    glm_vec3_copy(p->cam->rot, angle);
    angle[1] = 0.f;

    vec4 quat;
    util_eul2quat(angle, quat);

    vec3 front = { 1.f, 0.f, 0.f };
    glm_quat_rotatev(quat, front, front);
    glm_vec3_scale(front, move, front);

    vec3 right = { 0.f, 0.f, 1.f };
    glm_quat_rotatev(quat, right, right);
    glm_vec3_scale(right, move, right);

    if (glfwGetKey(p->win, GLFW_KEY_W) == GLFW_PRESS) glm_vec3_add(p->cam->pos, front, p->cam->pos);
    if (glfwGetKey(p->win, GLFW_KEY_S) == GLFW_PRESS) glm_vec3_sub(p->cam->pos, front, p->cam->pos);
    if (glfwGetKey(p->win, GLFW_KEY_A) == GLFW_PRESS) glm_vec3_sub(p->cam->pos, right, p->cam->pos);
    if (glfwGetKey(p->win, GLFW_KEY_D) == GLFW_PRESS) glm_vec3_add(p->cam->pos, right, p->cam->pos);

    if (glfwGetKey(p->win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) p->cam->pos[1] -= move;
    if (glfwGetKey(p->win, GLFW_KEY_SPACE) == GLFW_PRESS) p->cam->pos[1] += move;
}

