#include <avr/io.h>
#include <util/delay.h>

// TODO: napisać to po swojemu kiedyś, na razie jest gotowiec :P
// http://www.hobby.abxyz.bplaced.net/index.php?pid=4&aid=8

/* RS */
#define SET_OUT_LCD_RS  DDRB  |=  _BV(PB0)
#define SET_LCD_RS      PORTB |=  _BV(PB0)
#define CLR_LCD_RS      PORTB &= ~_BV(PB0)

/* RW */
#define SET_OUT_LCD_RW  DDRB  |=  _BV(PB1)
#define SET_LCD_RW      PORTB |=  _BV(PB1)
#define CLR_LCD_RW      PORTB &= ~_BV(PB1)

/* E */
#define SET_OUT_LCD_E   DDRB  |=  _BV(PB2)
#define SET_LCD_E       PORTB |=  _BV(PB2)
#define CLR_LCD_E       PORTB &= ~_BV(PB2)

/* D4 */
#define SET_OUT_LCD_D4  DDRC  |=  _BV(PC0)
#define SET_IN_LCD_D4   DDRC  &= ~_BV(PC0)
#define SET_LCD_D4      PORTC |=  _BV(PC0)
#define CLR_LCD_D4      PORTC &= ~_BV(PC0)
#define IS_SET_LCD_D4   PINC  &   _BV(PC0)

/* D5 */
#define SET_OUT_LCD_D5  DDRC  |=  _BV(PC1)
#define SET_IN_LCD_D5   DDRC  &= ~_BV(PC1)
#define SET_LCD_D5      PORTC |=  _BV(PC1)
#define CLR_LCD_D5      PORTC &= ~_BV(PC1)
#define IS_SET_LCD_D5   PINC  &   _BV(PC1)

/* D6 */
#define SET_OUT_LCD_D6  DDRC  |=  _BV(PC2)
#define SET_IN_LCD_D6   DDRC  &= ~_BV(PC2)
#define SET_LCD_D6      PORTC |=  _BV(PC2)
#define CLR_LCD_D6      PORTC &= ~_BV(PC2)
#define IS_SET_LCD_D6   PINC  &   _BV(PC2)

/* D7 */
#define SET_OUT_LCD_D7  DDRC  |=  _BV(PC3)
#define SET_IN_LCD_D7   DDRC  &= ~_BV(PC3)
#define SET_LCD_D7      PORTC |=  _BV(PC3)
#define CLR_LCD_D7      PORTC &= ~_BV(PC3)
#define IS_SET_LCD_D7   PINC  &   _BV(PC3)


#define LCD_NOP asm volatile("nop\n\t""nop\n\t" "nop\n\t" "nop\n\t" ::);



#define LCDCOMMAND 0
#define LCDDATA    1

#define LCD_LOCATE(x,y)  WriteToLCD(0x80|((x)+((y)*0x40)), LCDCOMMAND)

#define LCD_CLEAR              WriteToLCD(0x01, LCDCOMMAND)
#define LCD_HOME               WriteToLCD(0x02, LCDCOMMAND)

/* IDS */

#define LCDINCREMENT           0x02
#define LCDDECREMENT           0x00
#define LCDDISPLAYSHIFT        0x01

#define LCD_ENTRY_MODE(IDS)    WriteToLCD(0x04|(IDS), LCDCOMMAND)

/* BCD */
#define LCDDISPLAY             0x04
#define LCDCURSOR              0x02
#define LCDBLINK               0x01

#define LCD_DISPLAY(DCB)       WriteToLCD(0x08|(DCB), LCDCOMMAND)

/* RL */
#define LCDLEFT                0x00
#define LCDRIGHT               0x04

#define LCD_SHIFT_DISPLAY(RL)  WriteToLCD(0x18|(RL), LCDCOMMAND)
#define LCD_SHIFT_CURSOR(RL)   WriteToLCD(0x10|(RL), LCDCOMMAND)

#define LCD_CGRAM_ADDRESS(A)   WriteToLCD(0x40|((A)&0x3f), LCDCOMMAND)
#define LCD_DDRAM_ADDRESS(A)   WriteToLCD(0x80|((A)&0x7f), LCDCOMMAND)

#define LCD_WRITE_DATA(D)      WriteToLCD((D),LCDDATA)

/*--------------------------------------------------------*/
/* Zapis danej lub instrukcji */

void WriteToLCD (unsigned char v,unsigned char  rs)
{
    unsigned char bf;

    SET_OUT_LCD_D4;
    SET_OUT_LCD_D5;
    SET_OUT_LCD_D6;
    SET_OUT_LCD_D7;

    if(v&0x10) SET_LCD_D4; else CLR_LCD_D4;
    if(v&0x20) SET_LCD_D5; else CLR_LCD_D5;
    if(v&0x40) SET_LCD_D6; else CLR_LCD_D6;
    if(v&0x80) SET_LCD_D7; else CLR_LCD_D7;

    CLR_LCD_E;
    if(rs) SET_LCD_RS;else CLR_LCD_RS;
    CLR_LCD_RW;

    LCD_NOP;
    SET_LCD_E;
    LCD_NOP;
    CLR_LCD_E;
    LCD_NOP;

    if(v&0x01) SET_LCD_D4; else CLR_LCD_D4;
    if(v&0x02) SET_LCD_D5; else CLR_LCD_D5;
    if(v&0x04) SET_LCD_D6; else CLR_LCD_D6;
    if(v&0x08) SET_LCD_D7; else CLR_LCD_D7;

    LCD_NOP;
    SET_LCD_E;
    LCD_NOP;
    CLR_LCD_E;
    LCD_NOP;

    SET_IN_LCD_D4;
    SET_IN_LCD_D5;
    SET_IN_LCD_D6;
    SET_IN_LCD_D7;

    CLR_LCD_RS;
    SET_LCD_RW;
    SET_LCD_D7;


/* Przydałby się pełny odczyt */
    do
    {
        LCD_NOP;
        SET_LCD_E;
        LCD_NOP;
        bf = IS_SET_LCD_D7;
        CLR_LCD_E;
        LCD_NOP;
        SET_LCD_E;
        LCD_NOP;
        LCD_NOP;
        CLR_LCD_E;

    }while( bf );
}


/*--------------------------------------------------------*/
/* Funkcja odczytuje adres i flage zajetosci */

unsigned char ReadAddressLCD ( void)
{
    unsigned char g = 0 ;

    CLR_LCD_RS;
    SET_LCD_RW;

    SET_IN_LCD_D4;
    SET_IN_LCD_D5;
    SET_IN_LCD_D6;
    SET_IN_LCD_D7;

    LCD_NOP;
    SET_LCD_E;
    LCD_NOP;

    if(IS_SET_LCD_D4) g+=16;
    if(IS_SET_LCD_D4) g+=32;
    if(IS_SET_LCD_D4) g+=64;
    if(IS_SET_LCD_D4) g+=128;

    CLR_LCD_E;
    LCD_NOP;
    SET_LCD_E;
    LCD_NOP;

    if(IS_SET_LCD_D4) g+=8;
    if(IS_SET_LCD_D4) g+=4;
    if(IS_SET_LCD_D4) g+=2;
    if(IS_SET_LCD_D4) g+=1;

    CLR_LCD_E;

    return  g ;
}


/*---------------------------------------------------------*/
/* Inicjalizacja wyświetlacza */

void lcd_init(void)
{
    _delay_ms(31);

    SET_OUT_LCD_RS;
    SET_OUT_LCD_RW;
    SET_OUT_LCD_E;
    SET_OUT_LCD_D4;
    SET_OUT_LCD_D5;
    SET_OUT_LCD_D6;
    SET_OUT_LCD_D7;

    CLR_LCD_E;
    CLR_LCD_RS;
    CLR_LCD_RW;
    SET_LCD_D4;
    SET_LCD_D5;
    CLR_LCD_D6;
    CLR_LCD_D7;

    LCD_NOP;
    SET_LCD_E;
    LCD_NOP;
    CLR_LCD_E;
    LCD_NOP;
    _delay_ms(10);

    LCD_NOP;
    SET_LCD_E;
    LCD_NOP;
    CLR_LCD_E;
    LCD_NOP;
    _delay_ms(2);

    LCD_NOP;
    SET_LCD_E;
    LCD_NOP;
    CLR_LCD_E;
    LCD_NOP;
    _delay_ms(2);

    CLR_LCD_D4;
    LCD_NOP;
    SET_LCD_E;
    LCD_NOP;
    CLR_LCD_E;
    LCD_NOP;
    _delay_us(80);

    WriteToLCD (0x28 , LCDCOMMAND) ;
    LCD_DISPLAY(0) ;
    LCD_CLEAR ;
    LCD_ENTRY_MODE(LCDINCREMENT) ;
}

//////////////////////////////////////////////////////////////////////
#include "lcd.h"

void LCD::Init()
{
    lcd_init();
    TurnOn(true);
    MoveCursor(0, 0);
}

void LCD::ClearScreen()
{
    LCD_CLEAR;
}

void LCD::TurnOn(bool enableDisplay, bool enableCursor, bool enableCursorBlink)
{
    uint8_t data = 0;
    if(enableDisplay) data |= LCDDISPLAY;
    if(enableCursor) data |= LCDCURSOR;
    if(enableCursorBlink) data |= LCDBLINK;
    LCD_DISPLAY(data);
}

void LCD::MoveCursor(uint8_t x, uint8_t y)
{
    LCD_LOCATE(x, y);
}

void LCD::WriteChar(char c)
{
    LCD_WRITE_DATA(c);
}

void LCD::WriteCharAt(uint8_t x, uint8_t y, char c)
{
    MoveCursor(x, y);
    WriteChar(c);
}

void LCD::WriteString(const char* s)
{
    const char* ptr = s;
    while(*ptr != '\0')
        WriteChar(*ptr++);
}

void LCD::AddCustomCharacter(uint8_t character, const uint8_t image[8])
{
    WriteToLCD(0x40+character*8, LCDCOMMAND);
    for(uint8_t i = 0; i < 8; i++)
        WriteToLCD(image[i], LCDDATA);
}
