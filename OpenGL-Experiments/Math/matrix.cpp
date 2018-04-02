#include <algorithm>

#include "matrix.h"


mat2::mat2() {}
mat2::mat2(float diag):
           mat {diag, 0.0f,
                0.0f, diag}
{}
mat2::mat2(float amat[4]) {
    std::copy(amat, amat + 4, mat);
}
mat2::mat2(float aa, float ab,
           float ba, float bb):
           mat {aa, ab,
                ba, bb}
{}
float mat2::operator [](int i) const {
    return mat[i];
}
float &mat2::operator [](int i) {
    return mat[i];
}


mat3::mat3() {}
mat3::mat3(float diag):
           mat {diag, 0.0f, 0.0f,
                0.0f, diag, 0.0f,
                0.0f, 0.0f, diag}
{}
mat3::mat3(float amat[9]) {
    std::copy(amat, amat + 9, mat);
}
mat3::mat3(float aa, float ab, float ac,
           float ba, float bb, float bc,
           float ca, float cb, float cc):
           mat {aa, ab, ac,
                ba, bb, bc,
                ca, cb, cc}
{}
float mat3::operator [](int i) const {
    return mat[i];
}
float &mat3::operator [](int i) {
    return mat[i];
}


mat4::mat4() {}
mat4::mat4(float diag):
           mat {diag, 0.0f, 0.0f, 0.0f,
                0.0f, diag, 0.0f, 0.0f,
                0.0f, 0.0f, diag, 0.0f,
                0.0f, 0.0f, 0.0f, diag}
{}
mat4::mat4(float amat[16]) {
    std::copy(amat, amat + 16, mat);
}
mat4::mat4(float aa, float ab, float ac, float ad,
           float ba, float bb, float bc, float bd,
           float ca, float cb, float cc, float cd,
           float da, float db, float dc, float dd):
           mat {aa, ab, ac, ad,
                ba, bb, bc, bd,
                ca, cb, cc, cd,
                da, db, dc, dd}
{}
float mat4::operator [](int i) const {
    return mat[i];
}
float &mat4::operator [](int i) {
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
}
