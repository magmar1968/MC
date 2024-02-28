#ifndef FAST_EXP_H
#define FAST_EXP_H

#include <cstdint>
#include <cstring>


inline float fastExp(float x);
inline double fastExp(double x);



inline float fastExp(float x)
{
    constexpr float a = (1 << 23) / 0.69314718f;
    constexpr float b = (1 << 23) * (127 - 0.043677448f);
    x = a * x + b;

    constexpr float c = (1 << 23);
    constexpr float d = (1 << 23) * 255;
    if (x < c || x > d)
        x = (x < c) ? 0.0f : d;

    uint32_t n = static_cast<uint32_t>(x);
    memcpy(&x, &n, 4);
    return x;
}


inline double fastExp(double x)
{
    constexpr double a = (1ll << 52) / 0.6931471805599453;
    constexpr double b = (1ll << 52) * (1023 - 0.04367744890362246);
    x = a * x + b;

    constexpr double c = (1ll << 52);
    constexpr double d = (1ll << 52) * 2047;
    if (x < c || x > d)
        x = (x < c) ? 0.0 : d;

    uint64_t n = static_cast<uint64_t>(x);
    memcpy(&x, &n, 8);
    return x;
}

#endif