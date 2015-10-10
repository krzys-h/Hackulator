#pragma once

#include <stdint.h>

class LCD
{
public:
    static void Init();
    static void ClearScreen();
    static void TurnOn(bool enableDisplay, bool enableCursor = false, bool enableCursorBlink = false);
    static void MoveCursor(uint8_t x, uint8_t y);
    static void WriteChar(char c);
    static void WriteCharAt(uint8_t x, uint8_t y, char c);
    static void WriteString(const char* s);
    static void AddCustomCharacter(uint8_t character, const uint8_t image[8]);
};
