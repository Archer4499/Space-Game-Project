#include <algorithm>

#include "matrix.h"


// Because VS2013 doesn't implement all the features of C++11 I can't do:
// mat2::mat2(float diag):
//            mat {diag, 0.0f,
//                 0.0f, diag}
// {}



mat2::mat2() {}
mat2::mat2(float diag) {
    mat[0] = diag; mat[1] = 0.0f;
    mat[2] = 0.0f; mat[3] = diag;
}
mat2::mat2(float amat[4]) {
    std::copy(amat, amat + 4, mat);
}
mat2::mat2(float aa, float ab,
           float ba, float bb) {
    mat[0] = aa; mat[1] = ab;
    mat[2] = ba; mat[3] = bb;
}
float mat2::operator [](int i) const {
    return mat[i];
}
float& mat2::operator [](int i) {
    return mat[i];
}


mat3::mat3() {}
mat3::mat3(float diag) {
    mat[0] = diag; mat[1] = 0.0f; mat[2] = 0.0f;
    mat[3] = 0.0f; mat[4] = diag; mat[5] = 0.0f;
    mat[6] = 0.0f; mat[7] = 0.0f; mat[8] = diag;
}
mat3::mat3(float amat[9]) {
    std::copy(amat, amat + 9, mat);
}
mat3::mat3(float aa, float ab, float ac,
           float ba, float bb, float bc,
           float ca, float cb, float cc) {
    mat[0] = aa; mat[1] = ab; mat[2] = ac;
    mat[3] = ba; mat[4] = bb; mat[5] = bc;
    mat[6] = ca; mat[7] = cb; mat[8] = cc;
}
float mat3::operator [](int i) const {
    return mat[i];
}
float& mat3::operator [](int i) {
    return mat[i];
}


mat4::mat4() {}
mat4::mat4(float diag) {
    mat[0]  = diag; mat[1]  = 0.0f; mat[2]  = 0.0f; mat[3]  = 0.0f;
    mat[4]  = 0.0f; mat[5]  = diag; mat[6]  = 0.0f; mat[7]  = 0.0f;
    mat[8]  = 0.0f; mat[9]  = 0.0f; mat[10] = diag; mat[11] = 0.0f;
    mat[12] = 0.0f; mat[13] = 0.0f; mat[14] = 0.0f; mat[15] = diag;
}
mat4::mat4(float amat[16]) {
    std::copy(amat, amat + 16, mat);
}
mat4::mat4(float aa, float ab, float ac, float ad,
           float ba, float bb, float bc, float bd,
           float ca, float cb, float cc, float cd,
           float da, float db, float dc, float dd) {
    mat[0]  = aa; mat[1]  = ab; mat[2]  = ac; mat[3]  = ad;
    mat[4]  = ba; mat[5]  = bb; mat[6]  = bc; mat[7]  = bd;
    mat[8]  = ca; mat[9]  = cb; mat[10] = cc; mat[11] = cd;
    mat[12] = da; mat[13] = db; mat[14] = dc; mat[15] = dd;
}
float mat4::operator [](int i) const {
    return mat[i];
}
float& mat4::operator [](int i) {
    return mat[i];
}


std::ostream& operator<< (std::ostream& os, const mat2& mat) {
    os << "[[" << mat[0] << ", " << mat[1] << "], ["
               << mat[2] << ", " << mat[3] << "]]";
    return os;
}

std::ostream& operator<< (std::ostream& os, const mat3& mat) {
    os << "[[" << mat[0] << ", " << mat[1] << ", " << mat[2] << "], ["
               << mat[3] << ", " << mat[4] << ", " << mat[5] << "], ["
               << mat[6] << ", " << mat[7] << ", " << mat[8] << "]]";
    return os;
}

std::ostream& operator<< (std::ostream& os, const mat4& mat) {
    os << "[[" << mat[0]  << ", " << mat[1]  << ", " << mat[2]  << ", " << mat[3]  << "], ["
               << mat[4]  << ", " << mat[5]  << ", " << mat[6]  << ", " << mat[7]  << "], ["
               << mat[8]  << ", " << mat[9]  << ", " << mat[10] << ", " << mat[11] << "], ["
               << mat[12] << ", " << mat[13] << ", " << mat[14] << ", " << mat[15] << "]]";
    return os;
}


mat4 translate(mat4 trans, vec3 in) {
    mat4 out = mat4(trans);
    out[12] += in.x;
    out[13] += in.y;
    out[14] += in.z;

    return out;
}
