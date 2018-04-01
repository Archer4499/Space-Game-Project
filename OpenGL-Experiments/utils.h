#pragma once

struct vec2 {
  float x;
  float y;

  vec2();
  explicit vec2(float vert[]);
  vec2(float ax, float ay);
};

struct vec3 {
  float x;
  float y;
  float z;

  vec3();
  explicit vec3(float vert[]);
  vec3(float ax, float ay, float az);
};

void normalizeVector(const vec3 *v);
