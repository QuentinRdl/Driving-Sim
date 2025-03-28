#ifndef SCICALCHELPER_H
#define SCICALCHELPER_H
#include <cmath>


inline float radToDeg(const float rad) {
    return rad * 180.f / M_PIf;
}

inline float degToRad(const float deg) {
    return deg * M_PIf / 180.f;
}

#endif //SCICALCHELPER_H
