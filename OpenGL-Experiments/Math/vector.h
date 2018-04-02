#pragma once

struct vec2 {
  float x;
  float y;

  vec2();
  explicit vec2(float a);
  explicit vec2(float vert[]);
  vec2(float ax, float ay);
};

struct vec3 {
  float x;
  float y;
  float z;

  vec3();
  explicit vec3(float a);
  explicit vec3(float vert[]);
  vec3(float ax, float ay, float az);
};

struct vec4 {
  float x;
  float y;
  float z;
  float w;

  vec4();
  explicit vec4(float a);
  explicit vec4(float vert[]);
  vec4(float ax, float ay, float az, float w);
};


// void normalizeVector(const vec3 *v); TODO: not sure about syntax
