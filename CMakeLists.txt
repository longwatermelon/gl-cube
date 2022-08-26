cmake_minimum_required(VERSION 3.13.0)
set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(a.out)

add_executable(a.out
    src/main.c
    src/prog.h
    src/prog.c
    src/render.h
    src/render.c
    src/shader.h
    src/shader.c
    src/texture.h
    src/texture.c
    src/util.h
    src/util.c
    src/camera.h
    src/camera.c
)

target_include_directories(a.out PRIVATE deps/cglm/include deps/glad/include deps/glfw/include deps/stb_image)
target_link_directories(a.out PRIVATE deps)

target_compile_options(a.out PRIVATE -ggdb -Wall)
target_link_libraries(a.out PRIVATE glfw cglm m glad stb_image)
