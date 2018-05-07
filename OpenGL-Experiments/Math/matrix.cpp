#pragma warning(disable: 4996) // Visual C++ doesn't like std::copy with unknown array size e.g. &data[0][0]

#include <assert.h>
#include <algorithm>
#include <cmath>

#include "matrix.h"


// Because VS2013 doesn't implement all the features of C++11 I can't do:
// mat2::mat2(float diag):
//            mat {diag, 0.0f,
//                 0.0f, diag}
// {}
// But since my default initialiser doesn't do anything it doesn't really matter


mat2::mat2() {}
mat2::mat2(float diag) {
    data[0][0] = diag; data[1][0] = 0.0f;
    data[0][1] = 0.0f; data[1][1] = diag;
}
mat2::mat2(float amat[4]) {
    // NOTE: probably not guaranteed to actually work because of padding and things (but does for me)
    std::copy(amat, amat + 4, &data[0][0]);
    // data[0] = vec2(amat);
    // data[1] = vec2(amat+2);
}
mat2::mat2(vec2 x, vec2 y) {
    data[0] = x;
    data[1] = y;
}
mat2::mat2(float x0, float y0,
           float x1, float y1) {
    data[0][0] = x0; data[1][0] = y0;
    data[0][1] = x1; data[1][1] = y1;
}
vec2 mat2::operator[](int i) const {
    assert(i >= 0 && i < 2);
    return data[i];
}
vec2& mat2::operator[](int i) {
    assert(i >= 0 && i < 2);
    return data[i];
}
float mat2::access(int i) const {
    assert(i >= 0 && i < 4);
    return data[i/2][i%2];
}
float& mat2::access(int i) {
    assert(i >= 0 && i < 4);
    return data[i/2][i%2];
}

std::ostream& operator<< (std::ostream& os, const mat2& mat) {
    os << "\n [" << mat.access(0) << ", " << mat.access(2) << "]\n ["
                 << mat.access(1) << ", " << mat.access(3) << "]\n";
    return os;
}


mat3::mat3() {}
mat3::mat3(float diag) {
    data[0][0] = diag; data[1][0] = 0.0f; data[2][0] = 0.0f;
    data[0][1] = 0.0f; data[1][1] = diag; data[2][1] = 0.0f;
    data[0][2] = 0.0f; data[1][2] = 0.0f; data[2][2] = diag;
}
mat3::mat3(float amat[9]) {
    std::copy(amat, amat + 9, &data[0][0]);
}
mat3::mat3(vec3 x, vec3 y, vec3 z) {
    data[0] = x;
    data[1] = y;
    data[2] = z;
}
mat3::mat3(float x0, float y0, float z0,
           float x1, float y1, float z1,
           float x2, float y2, float z2) {
    data[0][0] = x0; data[1][0] = y0; data[2][0] = z0;
    data[0][1] = x1; data[1][1] = y1; data[2][1] = z1;
    data[0][2] = x2; data[1][2] = y2; data[2][2] = z2;
}
vec3 mat3::operator[](int i) const {
    assert(i >= 0 && i < 3);
    return data[i];
}
vec3& mat3::operator[](int i) {
    assert(i >= 0 && i < 3);
    return data[i];
}
float mat3::access(int i) const {
    assert(i >= 0 && i < 9);
    return data[i/3][i%3];
}
float& mat3::access(int i) {
    assert(i >= 0 && i < 9);
    return data[i/3][i%3];
}

std::ostream& operator<< (std::ostream& os, const mat3& mat) {
    os << "\n [" << mat.access(0) << ", " << mat.access(3) << ", " << mat.access(6) << "]\n ["
                 << mat.access(1) << ", " << mat.access(4) << ", " << mat.access(7) << "]\n ["
                 << mat.access(2) << ", " << mat.access(5) << ", " << mat.access(8) << "]\n";
    return os;
}


mat4::mat4() {}
mat4::mat4(float diag) {
    data[0][0] = diag; data[1][0] = 0.0f; data[2][0] = 0.0f; data[3][0] = 0.0f;
    data[0][1] = 0.0f; data[1][1] = diag; data[2][1] = 0.0f; data[3][1] = 0.0f;
    data[0][2] = 0.0f; data[1][2] = 0.0f; data[2][2] = diag; data[3][2] = 0.0f;
    data[0][3] = 0.0f; data[1][3] = 0.0f; data[2][3] = 0.0f; data[3][3] = diag;
}
mat4::mat4(float amat[16]) {
    std::copy(amat, amat + 16, &data[0][0]);
}
mat4::mat4(vec4 x, vec4 y, vec4 z, vec4 w) {
    data[0] = x;
    data[1] = y;
    data[2] = z;
    data[3] = w;
}
mat4::mat4(float x0, float y0, float z0, float w0,
           float x1, float y1, float z1, float w1,
           float x2, float y2, float z2, float w2,
           float x3, float y3, float z3, float w3) {
    data[0][0] = x0; data[1][0] = y0; data[2][0] = z0; data[3][0] = w0;
    data[0][1] = x1; data[1][1] = y1; data[2][1] = z1; data[3][1] = w1;
    data[0][2] = x2; data[1][2] = y2; data[2][2] = z2; data[3][2] = w2;
    data[0][3] = x3; data[1][3] = y3; data[2][3] = z3; data[3][3] = w3;
}
vec4 mat4::operator[](int i) const {
    assert(i >= 0 && i < 4);
    return data[i];
}
vec4& mat4::operator[](int i) {
    assert(i >= 0 && i < 4);
    return data[i];
}
float mat4::access(int i) const {
    assert(i >= 0 && i < 16);
    return data[i/4][i%4];
}
float& mat4::access(int i) {
    assert(i >= 0 && i < 16);
    return data[i/4][i%4];
}

std::ostream& operator<< (std::ostream& os, const mat4& mat) {
    os << "\n [" << mat.access(0) << ", " << mat.access(4) << ", " << mat.access(8)  << ", " << mat.access(12) << "]\n ["
                 << mat.access(1) << ", " << mat.access(5) << ", " << mat.access(9)  << ", " << mat.access(13) << "]\n ["
                 << mat.access(2) << ", " << mat.access(6) << ", " << mat.access(10) << ", " << mat.access(14) << "]\n ["
                 << mat.access(3) << ", " << mat.access(7) << ", " << mat.access(11) << ", " << mat.access(15) << "]\n";
    return os;
}


mat4 transpose(const mat4& in) {
    mat4 out;
    out[0][0] = in[0][0];
    out[0][1] = in[1][0];
    out[0][2] = in[2][0];
    out[0][3] = in[3][0];

    out[1][0] = in[0][1];
    out[1][1] = in[1][1];
    out[1][2] = in[2][1];
    out[1][3] = in[3][1];

    out[2][0] = in[0][2];
    out[2][1] = in[1][2];
    out[2][2] = in[2][2];
    out[2][3] = in[3][2];

    out[3][0] = in[0][3];
    out[3][1] = in[1][3];
    out[3][2] = in[2][3];
    out[3][3] = in[3][3];
    return out;
}

mat4 scale(const mat4& in, const vec3& s) {
    mat4 out(in);
    out[0] *= s.x;
    out[1] *= s.y;
    out[2] *= s.z;
    return out;
}

mat4 translate(const mat4& in, const vec3& trans) {
    mat4 out(in);
    out[3] += in[0] * trans.x + in[1] * trans.y + in[2] * trans.z;
    return out;
}

mat4 rotate(const mat4& in, float angle, const vec3& axis) {
    float c = cos(angle);
    float s = sin(angle);

    vec3 nAxis(normalize(axis));
    vec3 temp((1.0f - c) * nAxis);

    mat3 Rotate;
    Rotate[0][0] = c + temp[0] * nAxis[0];
    Rotate[0][1] = temp[0] * nAxis[1] + s * nAxis[2];
    Rotate[0][2] = temp[0] * nAxis[2] - s * nAxis[1];

    Rotate[1][0] = temp[1] * nAxis[0] - s * nAxis[2];
    Rotate[1][1] = c + temp[1] * nAxis[1];
    Rotate[1][2] = temp[1] * nAxis[2] + s * nAxis[0];

    Rotate[2][0] = temp[2] * nAxis[0] + s * nAxis[1];
    Rotate[2][1] = temp[2] * nAxis[1] - s * nAxis[0];
    Rotate[2][2] = c + temp[2] * nAxis[2];

    mat4 out;
    out[0] = in[0] * Rotate[0][0] + in[1] * Rotate[0][1] + in[2] * Rotate[0][2];
    out[1] = in[0] * Rotate[1][0] + in[1] * Rotate[1][1] + in[2] * Rotate[1][2];
    out[2] = in[0] * Rotate[2][0] + in[1] * Rotate[2][1] + in[2] * Rotate[2][2];
    out[3] = in[3];
    return out;
}

// Right-handed, depth: negative one to one
mat4 perspective(float fovy, float aspect, float zNear, float zFar) {
    float const tanHalfFovy = tan(fovy / 2.0f);

    mat4 out(0.0f);
    out[0][0] = 1.0f / (aspect * tanHalfFovy);
    out[1][1] = 1.0f / (tanHalfFovy);
    out[2][2] = - (zFar + zNear) / (zFar - zNear);
    out[2][3] = -1.0f;
    out[3][2] = - (2.0f * zFar * zNear) / (zFar - zNear);
    return out;
}

mat4 ortho(float left, float right, float bottom, float top) {
    mat4 out(1.0f);
    out[0][0] = 2.0f / (right - left);
    out[1][1] = 2.0f / (top - bottom);
    out[3][0] = - (right + left) / (right - left);
    out[3][1] = - (top + bottom) / (top - bottom);
    return out;
}

// Right-handed, depth: negative one to one
mat4 ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
    mat4 out(1.0f);
    out[0][0] =  2.0f / (right - left);
    out[1][1] =  2.0f / (top - bottom);
    out[2][2] = -2.0f / (zFar - zNear);
    out[3][0] = - (right + left) / (right - left);
    out[3][1] = - (top + bottom) / (top - bottom);
    out[3][2] = - (zFar + zNear) / (zFar - zNear);
    return out;
}

// Right-handed
mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up) {
    vec3 const f(normalize(center - eye));
    vec3 const s(normalize(cross(f, up)));
    vec3 const u(cross(s, f));

    mat4 out(1.0f);
    out[0][0] =  s.x;
    out[1][0] =  s.y;
    out[2][0] =  s.z;
    out[0][1] =  u.x;
    out[1][1] =  u.y;
    out[2][1] =  u.z;
    out[0][2] = -f.x;
    out[1][2] = -f.y;
    out[2][2] = -f.z;
    out[3][0] = -dot(s, eye);
    out[3][1] = -dot(u, eye);
    out[3][2] =  dot(f, eye);

    return out;
}
