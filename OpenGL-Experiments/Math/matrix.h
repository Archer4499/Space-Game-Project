#pragma once

#include "vector.h"

// Data stored in column-major order
// TODO(Derek): implement utility functions
// TODO(Derek): allow init using N vecNs

struct mat2 {
    float data[2][2];

    mat2();
    explicit mat2(float diag);
    explicit mat2(float amat[4]);
    mat2(float x0, float y0,
         float x1, float y1);

    float operator[](int i) const;
    float& operator[](int i);
};

std::ostream& operator<<(std::ostream& os, const mat2& mat);

inline bool  operator==(const mat2& A, const mat2& B) {bool ret = true; for (int i = 0; i < 4; ++i) ret &= (A[i]==B[i]); return ret;}
inline bool  operator!=(const mat2& A, const mat2& B) {return !operator==(A,B);}

inline mat2& operator- (mat2& A) {for (int i = 0; i < 4; ++i) A[i] = -A[i]; return A;}

// Matrix/Matrix
inline mat2& operator+=(mat2& A, const mat2& B) {for (int i = 0; i < 4; ++i) A[i] += B[i]; return A;}
inline mat2  operator+ (mat2  A, const mat2& B) {A += B; return A;}
inline mat2& operator-=(mat2& A, const mat2& B) {for (int i = 0; i < 4; ++i) A[i] -= B[i]; return A;}
inline mat2  operator- (mat2  A, const mat2& B) {A -= B; return A;}

// TODO(Derek): Check order of all matrix multiplies
// Matrix multiplication
inline mat2  operator* (const mat2& A, const mat2& B) {
    mat2 res(A.data[0][0] * B.data[0][0] + A.data[1][0] * B.data[0][1],
             A.data[0][1] * B.data[0][0] + A.data[1][1] * B.data[0][1],

             A.data[0][0] * B.data[1][0] + A.data[1][0] * B.data[1][1],
             A.data[0][1] * B.data[1][0] + A.data[1][1] * B.data[1][1]);
    return res;
}
// Horizontal/Row vector
inline vec2  operator* (vec2 A, const mat2& B) {
    vec2 res(A.x * B.data[0][0] + A.y * B.data[0][1],
             A.x * B.data[1][0] + A.y * B.data[1][1]);
    return res;
}
// Vertical/Column vector
inline vec2  operator* (const mat2& A, vec2 B) {
    vec2 res(A.data[0][0] * B.x + A.data[1][0] * B.y,
             A.data[0][1] * B.x + A.data[1][1] * B.y);
    return res;
}

// Scalar
inline mat2& operator*=(mat2& A, const float B) {for (int i = 0; i < 4; ++i) A[i] *= B; return A;}
inline mat2  operator* (mat2  A, const float B) {A *= B; return A;}
inline mat2  operator* (const float B, mat2  A) {A *= B; return A;}
inline mat2& operator/=(mat2& A, const float B) {for (int i = 0; i < 4; ++i) A[i] /= B; return A;}
inline mat2  operator/ (mat2  A, const float B) {A /= B; return A;}


struct mat3 {
    float data[3][3];

    mat3();
    explicit mat3(float diag);
    explicit mat3(float amat[9]);
    mat3(float x0, float y0, float z0,
         float x1, float y1, float z1,
         float x2, float y2, float z2);

    float operator[](int i) const;
    float& operator[](int i);
};

std::ostream& operator<<(std::ostream& os, const mat3& mat);

inline bool  operator==(const mat3& A, const mat3& B) {bool ret = true; for (int i = 0; i < 9; ++i) ret &= (A[i]==B[i]); return ret;}
inline bool  operator!=(const mat3& A, const mat3& B) {return !operator==(A,B);}

inline mat3& operator- (mat3& A) {for (int i = 0; i < 9; ++i) A[i] = -A[i]; return A;}

// Matrix/Matrix
inline mat3& operator+=(mat3& A, const mat3& B) {for (int i = 0; i < 9; ++i) A[i] += B[i]; return A;}
inline mat3  operator+ (mat3  A, const mat3& B) {A += B; return A;}
inline mat3& operator-=(mat3& A, const mat3& B) {for (int i = 0; i < 9; ++i) A[i] -= B[i]; return A;}
inline mat3  operator- (mat3  A, const mat3& B) {A -= B; return A;}

// Matrix multiplication
inline mat3  operator* (const mat3& A, const mat3& B) {
    mat3 res(A.data[0][0] * B.data[0][0] + A.data[1][0] * B.data[0][1] + A.data[2][0] * B.data[0][2],
             A.data[0][1] * B.data[0][0] + A.data[1][1] * B.data[0][1] + A.data[2][1] * B.data[0][2],
             A.data[0][2] * B.data[0][0] + A.data[1][2] * B.data[0][1] + A.data[2][2] * B.data[0][2],

             A.data[0][0] * B.data[1][0] + A.data[1][0] * B.data[1][1] + A.data[2][0] * B.data[1][2],
             A.data[0][1] * B.data[1][0] + A.data[1][1] * B.data[1][1] + A.data[2][1] * B.data[1][2],
             A.data[0][2] * B.data[1][0] + A.data[1][2] * B.data[1][1] + A.data[2][2] * B.data[1][2],

             A.data[0][0] * B.data[2][0] + A.data[1][0] * B.data[2][1] + A.data[2][0] * B.data[2][2],
             A.data[0][1] * B.data[2][0] + A.data[1][1] * B.data[2][1] + A.data[2][1] * B.data[2][2],
             A.data[0][2] * B.data[2][0] + A.data[1][2] * B.data[2][1] + A.data[2][2] * B.data[2][2]);
    return res;
}
// Horizontal/Row vector
inline vec3  operator* (vec3 A, const mat3& B) {
    vec3 res(A.x * B.data[0][0] + A.y * B.data[0][1] + A.z * B.data[0][2],
             A.x * B.data[1][0] + A.y * B.data[1][1] + A.z * B.data[1][2],
             A.x * B.data[2][0] + A.y * B.data[2][1] + A.z * B.data[2][2]);
    return res;
}
// Vertical/Column vector
inline vec3  operator* (const mat3& A, vec3 B) {
    vec3 res(A.data[0][0] * B.x + A.data[1][0] * B.y + A.data[2][0] * B.z,
             A.data[0][1] * B.x + A.data[1][1] * B.y + A.data[2][1] * B.z,
             A.data[0][2] * B.x + A.data[1][2] * B.y + A.data[2][2] * B.z);
    return res;
}

// Scalar
inline mat3& operator*=(mat3& A, const float B) {for (int i = 0; i < 9; ++i) A[i] *= B; return A;}
inline mat3  operator* (mat3  A, const float B) {A *= B; return A;}
inline mat3  operator* (const float B, mat3  A) {A *= B; return A;}
inline mat3& operator/=(mat3& A, const float B) {for (int i = 0; i < 9; ++i) A[i] /= B; return A;}
inline mat3  operator/ (mat3  A, const float B) {A /= B; return A;}


struct mat4 {
    float data[4][4];

    mat4();
    explicit mat4(float diag);
    explicit mat4(float amat[16]);
    mat4(float x0, float y0, float z0, float w0,
         float x1, float y1, float z1, float w1,
         float x2, float y2, float z2, float w2,
         float x3, float y3, float z3, float w3);

    float operator[](int i) const;
    float& operator[](int i);
};

std::ostream& operator<<(std::ostream& os, const mat4& mat);

inline bool  operator==(const mat4& A, const mat4& B) {bool ret = true; for (int i = 0; i < 16; ++i) ret &= (A[i]==B[i]); return ret;}
inline bool  operator!=(const mat4& A, const mat4& B) {return !operator==(A,B);}

inline mat4& operator- (mat4& A) {for (int i = 0; i < 16; ++i) A[i] = -A[i]; return A;}

// Matrix/Matrix
inline mat4& operator+=(mat4& A, const mat4& B) {for (int i = 0; i < 16; ++i) A[i] += B[i]; return A;}
inline mat4  operator+ (mat4  A, const mat4& B) {A += B; return A;}
inline mat4& operator-=(mat4& A, const mat4& B) {for (int i = 0; i < 16; ++i) A[i] -= B[i]; return A;}
inline mat4  operator- (mat4  A, const mat4& B) {A -= B; return A;}

// Matrix multiplication
inline mat4  operator* (const mat4& A, const mat4& B) {
    mat4 res(A.data[0][0] * B.data[0][0] + A.data[1][0] * B.data[0][1] + A.data[2][0] * B.data[0][2] + A.data[3][0] * B.data[0][3],
             A.data[0][1] * B.data[0][0] + A.data[1][1] * B.data[0][1] + A.data[2][1] * B.data[0][2] + A.data[3][1] * B.data[0][3],
             A.data[0][2] * B.data[0][0] + A.data[1][2] * B.data[0][1] + A.data[2][2] * B.data[0][2] + A.data[3][2] * B.data[0][3],
             A.data[0][3] * B.data[0][0] + A.data[1][3] * B.data[0][1] + A.data[2][3] * B.data[0][2] + A.data[3][3] * B.data[0][3],

             A.data[0][0] * B.data[1][0] + A.data[1][0] * B.data[1][1] + A.data[2][0] * B.data[1][2] + A.data[3][0] * B.data[1][3],
             A.data[0][1] * B.data[1][0] + A.data[1][1] * B.data[1][1] + A.data[2][1] * B.data[1][2] + A.data[3][1] * B.data[1][3],
             A.data[0][2] * B.data[1][0] + A.data[1][2] * B.data[1][1] + A.data[2][2] * B.data[1][2] + A.data[3][2] * B.data[1][3],
             A.data[0][3] * B.data[1][0] + A.data[1][3] * B.data[1][1] + A.data[2][3] * B.data[1][2] + A.data[3][3] * B.data[1][3],

             A.data[0][0] * B.data[2][0] + A.data[1][0] * B.data[2][1] + A.data[2][0] * B.data[2][2] + A.data[3][0] * B.data[2][3],
             A.data[0][1] * B.data[2][0] + A.data[1][1] * B.data[2][1] + A.data[2][1] * B.data[2][2] + A.data[3][1] * B.data[2][3],
             A.data[0][2] * B.data[2][0] + A.data[1][2] * B.data[2][1] + A.data[2][2] * B.data[2][2] + A.data[3][2] * B.data[2][3],
             A.data[0][3] * B.data[2][0] + A.data[1][3] * B.data[2][1] + A.data[2][3] * B.data[2][2] + A.data[3][3] * B.data[2][3],

             A.data[0][0] * B.data[3][0] + A.data[1][0] * B.data[3][1] + A.data[2][0] * B.data[3][2] + A.data[3][0] * B.data[3][3],
             A.data[0][1] * B.data[3][0] + A.data[1][1] * B.data[3][1] + A.data[2][1] * B.data[3][2] + A.data[3][1] * B.data[3][3],
             A.data[0][2] * B.data[3][0] + A.data[1][2] * B.data[3][1] + A.data[2][2] * B.data[3][2] + A.data[3][2] * B.data[3][3],
             A.data[0][3] * B.data[3][0] + A.data[1][3] * B.data[3][1] + A.data[2][3] * B.data[3][2] + A.data[3][3] * B.data[3][3]);
    return res;
}
// Horizontal/Row vector
inline vec4  operator* (vec4 A, const mat4& B) {
    vec4 res(A.x * B.data[0][0] + A.y * B.data[0][1] + A.z * B.data[0][2] + A.w * B.data[0][3],
             A.x * B.data[1][0] + A.y * B.data[1][1] + A.z * B.data[1][2] + A.w * B.data[1][3],
             A.x * B.data[2][0] + A.y * B.data[2][1] + A.z * B.data[2][2] + A.w * B.data[2][3],
             A.x * B.data[3][0] + A.y * B.data[3][1] + A.z * B.data[3][2] + A.w * B.data[3][3]);
    return res;
}
// Vertical/Column vector
inline vec4  operator* (const mat4& A, vec4 B) {
    vec4 res(A.data[0][0] * B.x + A.data[1][0] * B.y + A.data[2][0] * B.z + A.data[3][0] * B.w,
             A.data[0][1] * B.x + A.data[1][1] * B.y + A.data[2][1] * B.z + A.data[3][1] * B.w,
             A.data[0][2] * B.x + A.data[1][2] * B.y + A.data[2][2] * B.z + A.data[3][2] * B.w,
             A.data[0][3] * B.x + A.data[1][3] * B.y + A.data[2][3] * B.z + A.data[3][3] * B.w);
    return res;
}

// Scalar
inline mat4& operator*=(mat4& A, const float B) {for (int i = 0; i < 16; ++i) A[i] *= B; return A;}
inline mat4  operator* (mat4  A, const float B) {A *= B; return A;}
inline mat4  operator* (const float B, mat4  A) {A *= B; return A;}
inline mat4& operator/=(mat4& A, const float B) {for (int i = 0; i < 16; ++i) A[i] /= B; return A;}
inline mat4  operator/ (mat4  A, const float B) {A /= B; return A;}



mat4 translate(mat4 trans, vec3 in);