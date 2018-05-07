#include <assert.h>
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

float vec2::operator[](int i) const {
    assert(i >= 0 && i < 2);
    switch(i) {
        case 0:
            return x;
        case 1:
            return y;
    }
    return x; // Should never be here
}
float& vec2::operator[](int i) {
    assert(i >= 0 && i < 2);
    switch(i) {
        case 0:
            return x;
        case 1:
            return y;
    }
    return x; // Should never be here
}

std::ostream& operator<< (std::ostream& os, const vec2& vec) {
    os << "[" << vec.x << ", " << vec.y << "]";
    return os;
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

float vec3::operator[](int i) const {
    assert(i >= 0 && i < 3);
    switch(i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
    }
    return x; // Should never be here
}
float& vec3::operator[](int i) {
    assert(i >= 0 && i < 3);
    switch(i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
    }
    return x; // Should never be here
}

std::ostream& operator<< (std::ostream& os, const vec3& vec) {
    os << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]";
    return os;
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

float vec4::operator[](int i) const {
    assert(i >= 0 && i < 4);
    switch(i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
    }
    return x; // Should never be here
}
float& vec4::operator[](int i) {
    assert(i >= 0 && i < 4);
    switch(i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
    }
    return x; // Should never be here
}

std::ostream& operator<< (std::ostream& os, const vec4& vec) {
    os << "[" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << "]";
    return os;
}


vec3 normalize(vec3 v) {
    float len2 = v.x * v.x + v.y * v.y + v.z * v.z;

    if (len2 > 0.0f) {
        float len = sqrtf(len2);

        v.x /= len;
        v.y /= len;
        v.z /= len;
    }

    return v;
}

float dot(const vec2& a, const vec2& b) {
    vec2 tmp(a * b);
    return tmp.x + tmp.y;
}
float dot(const vec3& a, const vec3& b) {
    vec3 tmp(a * b);
    return tmp.x + tmp.y + tmp.z;
}
float dot(const vec4& a, const vec4& b) {
    vec4 tmp(a * b);
    return tmp.x + tmp.y + tmp.z + tmp.w;
}

vec3 cross(const vec3& a, const vec3& b) {
    return vec3(
        a.y * b.z - b.y * a.z,
        a.z * b.x - b.z * a.x,
        a.x * b.y - b.x * a.y);
}
