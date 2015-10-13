#include "math.h"

template<typename T>
T max(T x, T y)
{
    return (x > y) ? x : y;
}

/////////////

void number::alignComma(uint8_t pos)
{
    stripComma();
    //assert(pos >= commaPosition);
    while (commaPosition < pos)
    {
        value *= 10;
        commaPosition ++;
    }
}

void number::stripComma()
{
    if (value == 0)
    {
        commaPosition = 0;
        return;
    }

    while((value % 10) == 0 && commaPosition > 0)
    {
        value /= 10;
        commaPosition --;
    }
}

number number::operator-(number x)
{
    x.value = -x.value;
    return x;
}

number::operator double()
{
    double x = value;
    for(uint8_t i = 0; i < commaPosition; i++)
    {
        x /= 10;
    }
    return x;
}

number& operator+=(number& left, number right)
{
    int comma = max(left.commaPosition, right.commaPosition);
    left.alignComma(comma);
    right.alignComma(comma);

    left.value += right.value;

    left.stripComma();
    return left;
}

number& operator*=(number& left, number right)
{
    left.value *= right.value;
    left.commaPosition += right.commaPosition;

    left.stripComma();
    return left;
}

number& operator/=(number& left, number right)
{
    while(right.commaPosition > 0)
    {
        if(left.commaPosition > 0)
            left.commaPosition --;
        else
            left.value *= 10;

        right.commaPosition--;
    }

    int i = 0;
    while((left.value % right.value) != 0 && i++ < 10) // max 10 cyfr po przecinku jesli wynik jest niewymierny
    {
        left.value *= 10;
        left.commaPosition ++;
    }
    left.value /= right.value;

    left.stripComma();
    return left;
}
