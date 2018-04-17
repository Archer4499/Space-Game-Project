#pragma once

#include "vector.h"

// Data stored in column-major order
// TODO(Derek): implement utility functions
// TODO(Derek): allow init using N vecNs

struct mat2 {
    vec2 data[2];

    mat2();
    explicit mat2(float diag);
    explicit mat2(float amat[4]);
    mat2(vec2 x, vec2 y);
    mat2(float x0, float y0,
         float x1, float y1);

    vec2 operator[](int i) const;
    vec2& operator[](int i);

    float access(int i) const;
    float& access(int i);
};

std::ostream& operator<<(std::ostream& os, const mat2& mat);

// TODO(Derek): can use 2 vec2 comparisons now
inline bool  operator==(const mat2& A, const mat2& B) {return (A[0]==B[0] && A[1]==B[1]);}
inline bool  operator!=(const mat2& A, const mat2& B) {return !operator==(A,B);}

inline mat2  operator- (const mat2& A) {return mat2(-A[0], -A[1]);}

// Matrix/Matrix
inline mat2& operator+=(mat2& A, const mat2& B) {A[0] += B[0]; A[1] += B[1]; return A;}
inline mat2  operator+ (mat2  A, const mat2& B) {A += B; return A;}
inline mat2& operator-=(mat2& A, const mat2& B) {A[0] -= B[0]; A[1] -= B[1]; return A;}
inline mat2  operator- (mat2  A, const mat2& B) {A -= B; return A;}

// TODO(Derek): Check order of all matrix multiplies
// Matrix multiplication
inline mat2  operator* (const mat2& A, const mat2& B) {
    return mat2(A[0][0] * B[0][0] + A[1][0] * B[0][1],
                A[0][1] * B[0][0] + A[1][1] * B[0][1],

                A[0][0] * B[1][0] + A[1][0] * B[1][1],
                A[0][1] * B[1][0] + A[1][1] * B[1][1]);
}
// Horizontal/Row vector
inline vec2  operator* (vec2 A, const mat2& B) {
    return vec2(A.x * B[0][0] + A.y * B[0][1],
                A.x * B[1][0] + A.y * B[1][1]);
}
// Vertical/Column vector
inline vec2  operator* (const mat2& A, vec2 B) {return A[0] * B.x + A[1] * B.y;}

// Scalar
inline mat2& operator*=(mat2& A, const float B) {A[0] *= B; A[1] *= B; return A;}
inline mat2  operator* (mat2  A, const float B) {A *= B; return A;}
inline mat2  operator* (const float B, mat2  A) {A *= B; return A;}
inline mat2& operator/=(mat2& A, const float B) {A[0] /= B; A[1] /= B; return A;}
inline mat2  operator/ (mat2  A, const float B) {A /= B; return A;}


struct mat3 {
    vec3 data[3];

    mat3();
    explicit mat3(float diag);
    explicit mat3(float amat[9]);
    mat3(vec3 x, vec3 y, vec3 z);
    mat3(float x0, float y0, float z0,
         float x1, float y1, float z1,
         float x2, float y2, float z2);

    vec3 operator[](int i) const;
    vec3& operator[](int i);

    float access(int i) const;
    float& access(int i);
};

std::ostream& operator<<(std::ostream& os, const mat3& mat);

inline bool  operator==(const mat3& A, const mat3& B) {return (A[0]==B[0] && A[1]==B[1] && A[2]==B[2]);}
inline bool  operator!=(const mat3& A, const mat3& B) {return !operator==(A,B);}

inline mat3  operator- (const mat3& A) {return mat3(-A[0], -A[1], -A[2]);}

// Matrix/Matrix
inline mat3& operator+=(mat3& A, const mat3& B) {A[0] += B[0]; A[1] += B[1]; A[2] += B[2]; return A;}
inline mat3  operator+ (mat3  A, const mat3& B) {A += B; return A;}
inline mat3& operator-=(mat3& A, const mat3& B) {A[0] -= B[0]; A[1] -= B[1]; A[2] -= B[2]; return A;}
inline mat3  operator- (mat3  A, const mat3& B) {A -= B; return A;}

// Matrix multiplication
inline mat3  operator* (const mat3& A, const mat3& B) {
    return mat3(A[0][0] * B[0][0] + A[1][0] * B[0][1] + A[2][0] * B[0][2],
                A[0][1] * B[0][0] + A[1][1] * B[0][1] + A[2][1] * B[0][2],
                A[0][2] * B[0][0] + A[1][2] * B[0][1] + A[2][2] * B[0][2],

                A[0][0] * B[1][0] + A[1][0] * B[1][1] + A[2][0] * B[1][2],
                A[0][1] * B[1][0] + A[1][1] * B[1][1] + A[2][1] * B[1][2],
                A[0][2] * B[1][0] + A[1][2] * B[1][1] + A[2][2] * B[1][2],

                A[0][0] * B[2][0] + A[1][0] * B[2][1] + A[2][0] * B[2][2],
                A[0][1] * B[2][0] + A[1][1] * B[2][1] + A[2][1] * B[2][2],
                A[0][2] * B[2][0] + A[1][2] * B[2][1] + A[2][2] * B[2][2]);
}
// Horizontal/Row vector
inline vec3  operator* (vec3 A, const mat3& B) {
    return vec3(A.x * B[0][0] + A.y * B[0][1] + A.z * B[0][2],
                A.x * B[1][0] + A.y * B[1][1] + A.z * B[1][2],
                A.x * B[2][0] + A.y * B[2][1] + A.z * B[2][2]);
}
// Vertical/Column vector
inline vec3  operator* (const mat3& A, vec3 B) {return A[0] * B.x + A[1] * B.y + A[2] * B.z;}

// Scalar
inline mat3& operator*=(mat3& A, const float B) {A[0] *= B; A[1] *= B; A[2] *= B; return A;}
inline mat3  operator* (mat3  A, const float B) {A *= B; return A;}
inline mat3  operator* (const float B, mat3  A) {A *= B; return A;}
inline mat3& operator/=(mat3& A, const float B) {A[0] /= B; A[1] /= B; A[2] /= B; return A;}
inline mat3  operator/ (mat3  A, const float B) {A /= B; return A;}


struct mat4 {
    vec4 data[4];

    mat4();
    explicit mat4(float diag);
    explicit mat4(float amat[16]);
    mat4(vec4 x, vec4 y, vec4 z, vec4 w);
    mat4(float x0, float y0, float z0, float w0,
         float x1, float y1, float z1, float w1,
         float x2, float y2, float z2, float w2,
         float x3, float y3, float z3, float w3);

    vec4 operator[](int i) const;
    vec4& operator[](int i);

    float access(int i) const;
    float& access(int i);
};

std::ostream& operator<<(std::ostream& os, const mat4& mat);

inline bool  operator==(const mat4& A, const mat4& B) {return (A[0]==B[0] && A[1]==B[1] && A[2]==B[2] && A[3]==B[3]);}
inline bool  operator!=(const mat4& A, const mat4& B) {return !operator==(A,B);}

inline mat4  operator- (const mat4& A) {return mat4(-A[0], -A[1], -A[2], -A[3]);}

// Matrix/Matrix
inline mat4& operator+=(mat4& A, const mat4& B) {A[0] += B[0]; A[1] += B[1]; A[2] += B[2]; A[3] += B[3]; return A;}
inline mat4  operator+ (mat4  A, const mat4& B) {A += B; return A;}
inline mat4& operator-=(mat4& A, const mat4& B) {A[0] -= B[0]; A[1] -= B[1]; A[2] -= B[2]; A[3] -= B[3]; return A;}
inline mat4  operator- (mat4  A, const mat4& B) {A -= B; return A;}

// Matrix multiplication
inline mat4  operator* (const mat4& A, const mat4& B) {
    return mat4(A[0][0] * B[0][0] + A[1][0] * B[0][1] + A[2][0] * B[0][2] + A[3][0] * B[0][3],
                A[0][1] * B[0][0] + A[1][1] * B[0][1] + A[2][1] * B[0][2] + A[3][1] * B[0][3],
                A[0][2] * B[0][0] + A[1][2] * B[0][1] + A[2][2] * B[0][2] + A[3][2] * B[0][3],
                A[0][3] * B[0][0] + A[1][3] * B[0][1] + A[2][3] * B[0][2] + A[3][3] * B[0][3],

                A[0][0] * B[1][0] + A[1][0] * B[1][1] + A[2][0] * B[1][2] + A[3][0] * B[1][3],
                A[0][1] * B[1][0] + A[1][1] * B[1][1] + A[2][1] * B[1][2] + A[3][1] * B[1][3],
                A[0][2] * B[1][0] + A[1][2] * B[1][1] + A[2][2] * B[1][2] + A[3][2] * B[1][3],
                A[0][3] * B[1][0] + A[1][3] * B[1][1] + A[2][3] * B[1][2] + A[3][3] * B[1][3],

                A[0][0] * B[2][0] + A[1][0] * B[2][1] + A[2][0] * B[2][2] + A[3][0] * B[2][3],
                A[0][1] * B[2][0] + A[1][1] * B[2][1] + A[2][1] * B[2][2] + A[3][1] * B[2][3],
                A[0][2] * B[2][0] + A[1][2] * B[2][1] + A[2][2] * B[2][2] + A[3][2] * B[2][3],
                A[0][3] * B[2][0] + A[1][3] * B[2][1] + A[2][3] * B[2][2] + A[3][3] * B[2][3],

                A[0][0] * B[3][0] + A[1][0] * B[3][1] + A[2][0] * B[3][2] + A[3][0] * B[3][3],
                A[0][1] * B[3][0] + A[1][1] * B[3][1] + A[2][1] * B[3][2] + A[3][1] * B[3][3],
                A[0][2] * B[3][0] + A[1][2] * B[3][1] + A[2][2] * B[3][2] + A[3][2] * B[3][3],
                A[0][3] * B[3][0] + A[1][3] * B[3][1] + A[2][3] * B[3][2] + A[3][3] * B[3][3]);
}
// Horizontal/Row vector
inline vec4  operator* (vec4 A, const mat4& B) {
    return vec4(A.x * B[0][0] + A.y * B[0][1] + A.z * B[0][2] + A.w * B[0][3],
                A.x * B[1][0] + A.y * B[1][1] + A.z * B[1][2] + A.w * B[1][3],
                A.x * B[2][0] + A.y * B[2][1] + A.z * B[2][2] + A.w * B[2][3],
                A.x * B[3][0] + A.y * B[3][1] + A.z * B[3][2] + A.w * B[3][3]);
}
// Vertical/Column vector
inline vec4  operator* (const mat4& A, vec4 B) {return A[0] * B.x + A[1] * B.y + A[2] * B.z + A[3] * B.w;}

// Scalar
inline mat4& operator*=(mat4& A, const float B) {A[0] *= B; A[1] *= B; A[2] *= B; A[3] *= B; return A;}
inline mat4  operator* (mat4  A, const float B) {A *= B; return A;}
inline mat4  operator* (const float B, mat4  A) {A *= B; return A;}
inline mat4& operator/=(mat4& A, const float B) {A[0] /= B; A[1] /= B; A[2] /= B; A[3] /= B; return A;}
inline mat4  operator/ (mat4  A, const float B) {A /= B; return A;}



mat4 transpose(const mat4& in);
mat4 scale(const mat4& in, const vec3& s);
mat4 translate(const mat4& in, const vec3& trans);
mat4 rotate(const mat4& in, float angle, const vec3& axis);

/*
    det
    inverse
*/
