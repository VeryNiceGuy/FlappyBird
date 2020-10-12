#include "Utils.h"
#include <cmath>

float wrap(float x, float length) {
    float half = length / 2.0f;
    float v1 = ((x * -1.0) + half) / length;
    float v2 = (v1 - floorf(v1)) * length;
    float v3 = (v2 - half) * -1.0f;
    return v3;
}


float wrap2(float x, float gap) {
    if (x < 0)
        return (1024 + gap) - x;
    else return x;
}

/*
float wrap2(float x, float length) {
    float half = 67;
    float v1 = (x + half) / length;

    float v2 = (v1 - floorf(v1)) * length;
    float v3 = (v2 - half);
    return v3;
}*/

/*

float wrap(float x) {
    float length = 2048.0f;
    float half = length / 2.0f;
    float v1 = ((x * -1.0) + half) / length;
    float v2 = floorf(v1);
    float v3 = v1 - v2;
    float v4 = v3 * length;
    float v5 = (v4 - half) * -1.0f;
    return v5;
}
*/

/*
float wrap2(float x, float length, bool invert) {
    float sign = invert ? -1.0f : 1.0f;
    float half = length / 2.0f;
    float v1 = ((x * sign) + half) / length;
    float v2 = (v1 - floorf(v1)) * length;
    float v3 = (v2 - half) * sign;
    return v3;
}*/