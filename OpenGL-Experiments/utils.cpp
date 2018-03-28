#include <cmath>

#include "utils.h"


struct vec3 {
  float v[3];
  vec3() {
    v[0] = 0.0f;
    v[1] = 0.0f;
    v[2] = 0.0f;
  }
  vec3(float x, float y, float z) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
  }
};


void normalizeVector(vec3 *v) {
  float len2 = v->v[0] * v->v[0] + v->v[1] * v->v[1] + v->v[2] * v->v[2];
  if (len2 > 0.0f) {
    float len = sqrtf(len2);

    v->v[0] /= len;
    v->v[1] /= len;
    v->v[2] /= len;
  }
}