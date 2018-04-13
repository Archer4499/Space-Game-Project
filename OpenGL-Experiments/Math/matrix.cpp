#pragma warning(disable: 4996)

#include <assert.h>
#include <algorithm>

#include "matrix.h"


// Because VS2013 doesn't implement all the features of C++11 I can't do:
// mat2::mat2(float diag):
//            mat {diag, 0.0f,
//                 0.0f, diag}
// {}



mat2::mat2() {}
mat2::mat2(float diag) {
    data[0][0] = diag; data[1][0] = 0.0f;
    data[0][1] = 0.0f; data[1][1] = diag;
}
mat2::mat2(float amat[4]) {
    std::copy(amat, amat + 4, &data[0][0]);
}
mat2::mat2(float x0, float y0,
           float x1, float y1) {
    data[0][0] = x0; data[1][0] = y0;
    data[0][1] = x1; data[1][1] = y1;
}
float mat2::operator[](int i) const {
    assert(i >= 0 && i < 4);
    return data[0][i];
}
float& mat2::operator[](int i) {
    assert(i >= 0 && i < 4);
    return data[0][i];
}

std::ostream& operator<< (std::ostream& os, const mat2& mat) {
    os << "\n [" << mat[0] << ", " << mat[2] << "],\n ["
                 << mat[1] << ", " << mat[3] << "]";
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
mat3::mat3(float x0, float y0, float z0,
           float x1, float y1, float z1,
           float x2, float y2, float z2) {
    data[0][0] = x0; data[1][0] = y0; data[2][0] = z0;
    data[0][1] = x1; data[1][1] = y1; data[2][1] = z1;
    data[0][2] = x2; data[1][2] = y2; data[2][2] = z2;
}
float mat3::operator[](int i) const {
    assert(i >= 0 && i < 9);
    return data[0][i];
}
float& mat3::operator[](int i) {
    assert(i >= 0 && i < 9);
    return data[0][i];
}

std::ostream& operator<< (std::ostream& os, const mat3& mat) {
    os << "\n [" << mat[0] << ", " << mat[3] << ", " << mat[6] << "],\n ["
                 << mat[1] << ", " << mat[4] << ", " << mat[7] << "],\n ["
                 << mat[2] << ", " << mat[5] << ", " << mat[8] << "]";
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
mat4::mat4(float x0, float y0, float z0, float w0,
           float x1, float y1, float z1, float w1,
           float x2, float y2, float z2, float w2,
           float x3, float y3, float z3, float w3) {
    data[0][0] = x0; data[1][0] = y0; data[2][0] = z0; data[3][0] = w0;
    data[0][1] = x1; data[1][1] = y1; data[2][1] = z1; data[3][1] = w1;
    data[0][2] = x2; data[1][2] = y2; data[2][2] = z2; data[3][2] = w2;
    data[0][3] = x3; data[1][3] = y3; data[2][3] = z3; data[3][3] = w3;
}
float mat4::operator[](int i) const {
    assert(i >= 0 && i < 16);
    return data[0][i];
}
float& mat4::operator[](int i) {
    assert(i >= 0 && i < 16);
    return data[0][i];
}

std::ostream& operator<< (std::ostream& os, const mat4& mat) {
    os << "\n [" << mat[0] << ", " << mat[4] << ", " << mat[8]  << ", " << mat[12] << "],\n ["
                 << mat[1] << ", " << mat[5] << ", " << mat[9]  << ", " << mat[13] << "],\n ["
                 << mat[2] << ", " << mat[6] << ", " << mat[10] << ", " << mat[14] << "],\n ["
                 << mat[3] << ", " << mat[7] << ", " << mat[11] << ", " << mat[15] << "]";
    return os;
}



mat4 translate(mat4 trans, vec3 in) {
    mat4 out = mat4(trans);
    out[12] += in.x;
    out[13] += in.y;
    out[14] += in.z;

    return out;
}
