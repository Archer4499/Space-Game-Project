#define PI 3.14159265359f

float degrees(float radians) {
    return radians * (180.0f/PI);
}
float radians(float degrees) {
    return degrees * (PI/180.0f);
}
