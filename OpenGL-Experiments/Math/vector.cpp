#include <cmath>

#include "vector.h"


vec2::vec2() {
    x = 0.0f;
    y = 0.0f;
}
vec2::vec2(float a) {
    x = a;
    y = a;
}
vec2::vec2(float vert[2]) {
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
vec3::vec3(float a) {
    x = a;
    y = a;
    z = a;
}
vec3::vec3(float vert[3]) {
    x = vert[0];
    y = vert[1];
    z = vert[2];
}
vec3::vec3(float ax, float ay, float az) {
    x = ax;
    y = ay;
    z = az;
}

vec4::vec4() {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 0.0f;
}
vec4::vec4(float a) {
    x = a;
    y = a;
    z = a;
    w = a;
}
vec4::vec4(float vert[4]) {
    x = vert[0];
    y = vert[1];
    z = vert[2];
    w = vert[3];
}
vec4::vec4(float ax, float ay, float az, float aw) {
    x = ax;
    y = ay;
    z = az;
    w = aw;
}


std::ostream& operator<< (std::ostream& os, const vec2& vec) {
    os << "[" << vec.x << ", " << vec.y << "]";
    return os;
}

std::ostream& operator<< (std::ostream& os, const vec3& vec) {
    os << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]";
    return os;
}

std::ostream& operator<< (std::ostream& os, const vec4& vec) {
    os << "[" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << "]";
    return os;
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