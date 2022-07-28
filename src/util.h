#ifndef UTIL_H
#define UTIL_H

#include <cglm/cglm.h>

#define SCRW 800.f
#define SCRH 600.f

char *util_read_file(const char *path);

void util_quat_from_rot(vec3 rot, vec4 dest);
void util_eul2quat(vec3 rot, vec4 dest);

#endif

