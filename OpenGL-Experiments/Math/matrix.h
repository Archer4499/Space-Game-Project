#pragma once

#include "vector.h"

// Data stored in column-major order
// TODO(Derek): implement utility functions
// TODO(Derek): mat*mat, mat*vec, vec*mat

struct mat2 {
    float mat[4];

    mat2();
    explicit mat2(float diag);
    explicit mat2(float amat[4]);
    mat2(float aa, float ab,
         float ba, float bb);

    float operator[](int i) const;
    float& operator[](int i);
};

std::ostream& operator<<(std::ostream& os, const mat2& mat);

inline bool  operator==(const mat2& A, const mat2& B) {bool ret = true; for (int i = 0; i < 4; ++i) ret &= (A[i]==B[i]); return ret;}
inline bool  operator!=(const mat2& A, const mat2& B) {return !operator==(A,B);}

inline mat2& operator- (mat2& A) {for (int i = 0; i < 4; ++i) A[i] = -A[i]; return A;}

inline mat2& operator+=(mat2& A, const mat2& B) {for (int i = 0; i < 4; ++i) A[i] += A[i]; return A;}
inline mat2  operator+ (mat2  A, const mat2& B) {A += B; return A;}
inline mat2& operator-=(mat2& A, const mat2& B) {for (int i = 0; i < 4; ++i) A[i] -= A[i]; return A;}
inline mat2  operator- (mat2  A, const mat2& B) {A -= B; return A;}

inline mat2& operator*=(mat2& A, const float B) {for (int i = 0; i < 4; ++i) A[i] *= A[i]; return A;}
inline mat2  operator* (mat2  A, const float B) {A *= B; return A;}
inline mat2  operator* (const float B, mat2  A) {A *= B; return A;}
inline mat2& operator/=(mat2& A, const float B) {for (int i = 0; i < 4; ++i) A[i] /= A[i]; return A;}
inline mat2  operator/ (mat2  A, const float B) {A /= B; return A;}


struct mat3 {
    float mat[9];

    mat3();
    explicit mat3(float diag);
    explicit mat3(float amat[9]);
    mat3(float aa, float ab, float ac,
         float ba, float bb, float bc,
         float ca, float cb, float cc);

    float operator[](int i) const;
    float& operator[](int i);
};

std::ostream& operator<<(std::ostream& os, const mat3& mat);

inline bool  operator==(const mat3& A, const mat3& B) {bool ret = true; for (int i = 0; i < 4; ++i) ret &= (A[i]==B[i]); return ret;}
inline bool  operator!=(const mat3& A, const mat3& B) {return !operator==(A,B);}

inline mat3& operator- (mat3& A) {for (int i = 0; i < 4; ++i) A[i] = -A[i]; return A;}

inline mat3& operator+=(mat3& A, const mat3& B) {for (int i = 0; i < 4; ++i) A[i] += A[i]; return A;}
inline mat3  operator+ (mat3  A, const mat3& B) {A += B; return A;}
inline mat3& operator-=(mat3& A, const mat3& B) {for (int i = 0; i < 4; ++i) A[i] -= A[i]; return A;}
inline mat3  operator- (mat3  A, const mat3& B) {A -= B; return A;}

inline mat3& operator*=(mat3& A, const float B) {for (int i = 0; i < 4; ++i) A[i] *= A[i]; return A;}
inline mat3  operator* (mat3  A, const float B) {A *= B; return A;}
inline mat3  operator* (const float B, mat3  A) {A *= B; return A;}
inline mat3& operator/=(mat3& A, const float B) {for (int i = 0; i < 4; ++i) A[i] /= A[i]; return A;}
inline mat3  operator/ (mat3  A, const float B) {A /= B; return A;}


struct mat4 {
    float mat[16];

    mat4();
    explicit mat4(float diag);
    explicit mat4(float amat[16]);
    mat4(float aa, float ab, float ac, float ad,
         float ba, float bb, float bc, float bd,
         float ca, float cb, float cc, float cd,
         float da, float db, float dc, float dd);

    float operator[](int i) const;
    float& operator[](int i);
};

std::ostream& operator<<(std::ostream& os, const mat4& mat);

inline bool  operator==(const mat4& A, const mat4& B) {bool ret = true; for (int i = 0; i < 4; ++i) ret &= (A[i]==B[i]); return ret;}
inline bool  operator!=(const mat4& A, const mat4& B) {return !operator==(A,B);}

inline mat4& operator- (mat4& A) {for (int i = 0; i < 4; ++i) A[i] = -A[i]; return A;}

inline mat4& operator+=(mat4& A, const mat4& B) {for (int i = 0; i < 4; ++i) A[i] += A[i]; return A;}
inline mat4  operator+ (mat4  A, const mat4& B) {A += B; return A;}
inline mat4& operator-=(mat4& A, const mat4& B) {for (int i = 0; i < 4; ++i) A[i] -= A[i]; return A;}
inline mat4  operator- (mat4  A, const mat4& B) {A -= B; return A;}

inline mat4& operator*=(mat4& A, const float B) {for (int i = 0; i < 4; ++i) A[i] *= A[i]; return A;}
inline mat4  operator* (mat4  A, const float B) {A *= B; return A;}
inline mat4  operator* (const float B, mat4  A) {A *= B; return A;}
inline mat4& operator/=(mat4& A, const float B) {for (int i = 0; i < 4; ++i) A[i] /= A[i]; return A;}
inline mat4  operator/ (mat4  A, const float B) {A /= B; return A;}



mat4 translate(mat4 trans, vec3 in);