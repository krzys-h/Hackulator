#pragma once

#include <stdint.h>

struct number
{
    int64_t value;
    uint8_t commaPosition;

    number(int64_t value = 0, uint8_t commaPosition = 0)
    : value(value), commaPosition(commaPosition)
    {}

    void alignComma(uint8_t pos);
    void stripComma();

    number operator-(number x);
    operator double();
};

number& operator+=(number& left, number right);
inline number operator+(number left, number right) { return left += right; }

inline number& operator-=(number& left, number right)
{
    return left += -right;
}
inline number operator-(number left, number right) { return left -= right; }

number& operator*=(number& left, number right);
inline number operator*(number left, number right) { return left *= right; }

number& operator/=(number& left, number right);
inline number operator/(number left, number right) { return left /= right; }
