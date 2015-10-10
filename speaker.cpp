#include "speaker.h"

#include <avr/io.h>
#include <util/delay.h>

void Speaker::Init()
{
    DDRC  |= _BV(4);
    PORTC |= _BV(4);
}

void Speaker::Beep(uint16_t freq, uint16_t delay)
{
    uint16_t t = 125000/freq;
    uint16_t n = (250UL*delay)/t;

    PORTC |= _BV(4);
    for (uint16_t i = 0; i < n; i++)
    {
        PORTC ^= _BV(4);
        _delay_loop_2(t);
    }
    PORTC |= _BV(4);
}

void Speaker::Beep(Sound sound)
{
    Beep(sound.freq, sound.delay);
}

void Speaker::Play(Sound sounds[])
{
    for(uint8_t i = 0; sounds[i].IsValid(); i++)
    {
        Beep(sounds[i]);
    }
}
