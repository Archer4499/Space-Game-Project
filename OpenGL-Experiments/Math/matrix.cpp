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



mat4 translate(mat4 trans, vec3 in) {
    mat4 out = mat4(trans);
    out[12] += in.x;
    out[13] += in.y;
    out[14] += in.z;

    return out;
}
