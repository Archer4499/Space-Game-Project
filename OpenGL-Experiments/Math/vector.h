#pragma once

#include <ostream>

// TODO(Derek): implement format_arg with floating point precision and raw/pretty print as input
// TODO(Derek): implement utility functions
// NOTE: https://stackoverflow.com/questions/4421706/what-are-the-basic-rules-and-idioms-for-operator-overloading
// mag
// magSquared


struct vec2 {
    float x, y;

    vec2();
    explicit vec2(float a);
    explicit vec2(float vert[2]);
    vec2(float ax, float ay);

    float  operator[](int i) const;
    float& operator[](int i);
};

std::ostream& operator<<(std::ostream& os, const vec2& vec);

// Exact float comparison
inline bool  operator==(const vec2& A, const vec2& B) {return (A.x == B.x) && (A.y == B.y);}
inline bool  operator!=(const vec2& A, const vec2& B) {return !operator==(A,B);}
// TODO(Derek): Not sure what (if anything) these would mean
// inline bool  operator< (const vec2& A, const vec2& B) {return ;}
// inline bool  operator> (const vec2& A, const vec2& B) {return  operator< (B,A);}
// inline bool  operator<=(const vec2& A, const vec2& B) {return !operator> (A,B);}
// inline bool  operator>=(const vec2& A, const vec2& B) {return !operator< (A,B);}

inline vec2  operator- (const vec2& A) {return vec2(-A.x, -A.y);}

// Component-wise operations
inline vec2& operator+=(vec2& A, const vec2& B) {A.x += B.x; A.y += B.y; return A;}
inline vec2  operator+ (vec2  A, const vec2& B) {A += B; return A;}
inline vec2& operator-=(vec2& A, const vec2& B) {A.x -= B.x; A.y -= B.y; return A;}
inline vec2  operator- (vec2  A, const vec2& B) {A -= B; return A;}
inline vec2& operator*=(vec2& A, const vec2& B) {A.x *= B.x; A.y *= B.y; return A;}
inline vec2  operator* (vec2  A, const vec2& B) {A *= B; return A;}
inline vec2& operator/=(vec2& A, const vec2& B) {A.x /= B.x; A.y /= B.y; return A;}
inline vec2  operator/ (vec2  A, const vec2& B) {A /= B; return A;}

inline vec2& operator*=(vec2& A, const float B) {A.x *= B; A.y *= B; return A;}
inline vec2  operator* (vec2  A, const float B) {A *= B; return A;}
inline vec2  operator* (const float B, vec2  A) {A *= B; return A;}
inline vec2& operator/=(vec2& A, const float B) {A.x /= B; A.y /= B; return A;}
inline vec2  operator/ (vec2  A, const float B) {A /= B; return A;}
// inline vec2  operator/ (const float B, vec2  A) {A /= B; return A;}


struct vec3 {
    float x, y, z;

    vec3();
    explicit vec3(float a);
    explicit vec3(float vert[3]);
    vec3(float ax, float ay, float az);

    float  operator[](int i) const;
    float& operator[](int i);
};

std::ostream& operator<<(std::ostream& os, const vec3& vec);

inline bool  operator==(const vec3& A, const vec3& B) {return (A.x == B.x) && (A.y == B.y) && (A.z == B.z);}
inline bool  operator!=(const vec3& A, const vec3& B) {return !operator==(A,B);}

inline vec3  operator- (const vec3& A) {return vec3(-A.x, -A.y, -A.z);}

// Component-wise operations
inline vec3& operator+=(vec3& A, const vec3& B) {A.x += B.x; A.y += B.y; A.z += B.z; return A;}
inline vec3  operator+ (vec3  A, const vec3& B) {A += B; return A;}
inline vec3& operator-=(vec3& A, const vec3& B) {A.x -= B.x; A.y -= B.y; A.z -= B.z; return A;}
inline vec3  operator- (vec3  A, const vec3& B) {A -= B; return A;}
inline vec3& operator*=(vec3& A, const vec3& B) {A.x *= B.x; A.y *= B.y; A.z *= B.z; return A;}
inline vec3  operator* (vec3  A, const vec3& B) {A *= B; return A;}
inline vec3& operator/=(vec3& A, const vec3& B) {A.x /= B.x; A.y /= B.y; A.z /= B.z; return A;}
inline vec3  operator/ (vec3  A, const vec3& B) {A /= B; return A;}

inline vec3& operator*=(vec3& A, const float B) {A.x *= B; A.y *= B; A.z *= B; return A;}
inline vec3  operator* (vec3  A, const float B) {A *= B; return A;}
inline vec3  operator* (const float B, vec3  A) {A *= B; return A;}
inline vec3& operator/=(vec3& A, const float B) {A.x /= B; A.y /= B; A.z /= B; return A;}
inline vec3  operator/ (vec3  A, const float B) {A /= B; return A;}


struct vec4 {
    float x, y, z, w;

    vec4();
    explicit vec4(float a);
    explicit vec4(float vert[4]);
    vec4(float ax, float ay, float az, float w);

    float  operator[](int i) const;
    float& operator[](int i);
};

std::ostream& operator<<(std::ostream&, const vec4&);

inline bool  operator==(const vec4& A, const vec4& B) {return (A.x == B.x) && (A.y == B.y) && (A.z == B.z) && (A.w == B.w);}
inline bool  operator!=(const vec4& A, const vec4& B) {return !operator==(A,B);}

inline vec4  operator- (const vec4& A) {return vec4(-A.x, -A.y, -A.z, -A.w);}

// Component-wise operations
inline vec4& operator+=(vec4& A, const vec4& B) {A.x += B.x; A.y += B.y; A.z += B.z; A.w += B.w; return A;}
inline vec4  operator+ (vec4  A, const vec4& B) {A += B; return A;}
inline vec4& operator-=(vec4& A, const vec4& B) {A.x -= B.x; A.y -= B.y; A.z -= B.z; A.w -= B.w; return A;}
inline vec4  operator- (vec4  A, const vec4& B) {A -= B; return A;}
inline vec4& operator*=(vec4& A, const vec4& B) {A.x *= B.x; A.y *= B.y; A.z *= B.z; A.w *= B.w; return A;}
inline vec4  operator* (vec4  A, const vec4& B) {A *= B; return A;}
inline vec4& operator/=(vec4& A, const vec4& B) {A.x /= B.x; A.y /= B.y; A.z /= B.z; A.w /= B.w; return A;}
inline vec4  operator/ (vec4  A, const vec4& B) {A /= B; return A;}

inline vec4& operator*=(vec4& A, const float B) {A.x *= B; A.y *= B; A.z *= B; A.w *= B; return A;}
inline vec4  operator* (vec4  A, const float B) {A *= B; return A;}
inline vec4  operator* (const float B, vec4  A) {A *= B; return A;}
inline vec4& operator/=(vec4& A, const float B) {A.x /= B; A.y /= B; A.z /= B; A.w /= B; return A;}
inline vec4  operator/ (vec4  A, const float B) {A /= B; return A;}


vec3 normalize(vec3 v);
float dot(const vec2& a, const vec2& b);
float dot(const vec3& a, const vec3& b);
float dot(const vec4& a, const vec4& b);
vec3 cross(const vec3& a, const vec3& b);
