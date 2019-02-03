#include "func.h"

float degrees(float radians) {
    return radians * (180.0f/PI);
}
float radians(float degrees) {
    return degrees * (PI/180.0f);
}

float lerp(float a, float b, float value) {
    return a + (b - a) * value;
}

float clamp(float value, float min, float max) {
    if (value > max)
        return max;
    else if (value < min)
        return min;
    else
        return value;
}
