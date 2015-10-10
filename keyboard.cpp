#include "keyboard.h"
#include <avr/io.h>
#include <util/delay.h>
#define BAUD 4800

// TODO: podpiąć klawiaturę, ten kod przerzucić do obsługi komunikacji

void Keyboard::Init()
{
    uint16_t ubbr = (F_CPU)/(BAUD*16UL)-1;
    UBRRH = (uint8_t)(ubbr>>8);
    UBRRL = (uint8_t)ubbr;

    UCSRB = (1<<RXEN) | (1<<TXEN);
}

Key Keyboard::GetKey()
{
    while(!(UCSRA & (1<<RXC)));
    uint8_t k = UDR;
    if (k >= '0' && k <= '9') return (Key)(k-'0');
    if (k == '\r') return KEY_EQUALS;
    return (Key)k;
}
