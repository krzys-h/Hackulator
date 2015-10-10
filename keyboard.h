#pragma once

enum Key
{
    KEY_0 = 0,
    KEY_1 = 1,
    KEY_2 = 2,
    KEY_3 = 3,
    KEY_4 = 4,
    KEY_5 = 5,
    KEY_6 = 6,
    KEY_7 = 7,
    KEY_8 = 8,
    KEY_9 = 9,
    KEY_COMMA = '.',
    KEY_PLUS_MINUS = 'X',
    KEY_ADD = '+',
    KEY_SUB = '-',
    KEY_MUL = '*',
    KEY_DIV = '/',
    KEY_EQUALS = '=',
    KEY_PERCENT = '%',
    KEY_SQRT = 'S',
    KEY_ON = '>',
    KEY_OFF = '<',
    KEY_SOUND = ';',
    KEY_M_ADD,
    KEY_M_SUB,
    KEY_M_READ,
    KEY_M_CLEAR,
};

class Keyboard
{
public:
    static void Init();
    static Key GetKey();
};
