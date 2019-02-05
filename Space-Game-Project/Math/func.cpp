#include "func.h"

int max(int a, int b) {
    return (a<b)?b:a;
}
int min(int a, int b) {
    return (a>b)?b:a;
}


float degrees(float radians) {
    return radians * (180.0f/PI);
}
float radians(float degrees) {
    return degrees * (PI/180.0f);
}

float max(float a, float b) {
    return (a<b)?b:a;
}
float min(float a, float b) {
    return (a>b)?b:a;
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


double max(double a, double b) {
    return (a<b)?b:a;
}
double min(double a, double b) {
    return (a>b)?b:a;
}
