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
