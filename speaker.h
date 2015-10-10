#pragma once

#include <stdint.h>

struct Sound
{
    uint16_t freq = 0xFFFF;
    uint16_t delay = 0xFFFF;

    Sound() {}
    Sound(uint16_t freq, uint16_t delay) : freq(freq), delay(delay) {}

    bool IsValid()
    {
        return freq != 0xFFFF && delay != 0xFFFF;
    }
};

class Speaker
{
public:
    static void Init();
    static void Beep(uint16_t freq, uint16_t delay = 250);
    static void Beep(Sound sound);
    static void Play(Sound sounds[]);
};
