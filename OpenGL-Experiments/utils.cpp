#include <cmath>
#include <vector>

#include "utils.h"


vec2::vec2() {
    x = 0.0f;
    y = 0.0f;
}
vec2::vec2(float vert[]) {
    x = vert[0];
    y = vert[1];
}
vec2::vec2(float ax, float ay) {
    x = ax;
    y = ay;
}


vec3::vec3() {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}
vec3::vec3(float vert[]) {
    x = vert[0];
    y = vert[1];
    z = vert[2];
}
vec3::vec3(float ax, float ay, float az) {
    x = ax;
    y = ay;
    z = az;
}


void normalizeVector(vec3 *v) {
  float len2 = v->x * v->x + v->y * v->y + v->z * v->z;
  if (len2 > 0.0f) {
    float len = sqrtf(len2);

    v->x /= len;
    v->y /= len;
    v->z /= len;
  }
}