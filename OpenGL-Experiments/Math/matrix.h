#pragma once

#include "vector.h"

// Data stored in column-major order
// TODO(Derek): implement utility functions
// TODO(Derek): mat*mat, mat*vec, vec*mat
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

inline mat3& operator+=(mat3& A, const mat3& B) {for (int i = 0; i < 9; ++i) A[i] += B[i]; return A;}
inline mat3  operator+ (mat3  A, const mat3& B) {A += B; return A;}
inline mat3& operator-=(mat3& A, const mat3& B) {for (int i = 0; i < 9; ++i) A[i] -= B[i]; return A;}
inline mat3  operator- (mat3  A, const mat3& B) {A -= B; return A;}

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

inline mat4& operator+=(mat4& A, const mat4& B) {for (int i = 0; i < 16; ++i) A[i] += B[i]; return A;}
inline mat4  operator+ (mat4  A, const mat4& B) {A += B; return A;}
inline mat4& operator-=(mat4& A, const mat4& B) {for (int i = 0; i < 16; ++i) A[i] -= B[i]; return A;}
inline mat4  operator- (mat4  A, const mat4& B) {A -= B; return A;}

inline mat4& operator*=(mat4& A, const float B) {for (int i = 0; i < 16; ++i) A[i] *= B; return A;}
inline mat4  operator* (mat4  A, const float B) {A *= B; return A;}
inline mat4  operator* (const float B, mat4  A) {A *= B; return A;}
inline mat4& operator/=(mat4& A, const float B) {for (int i = 0; i < 16; ++i) A[i] /= B; return A;}
inline mat4  operator/ (mat4  A, const float B) {A /= B; return A;}



mat4 translate(mat4 trans, vec3 in);