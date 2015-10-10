#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "lcd.h"
#include "keyboard.h"
#include "speaker.h"

int64_t result = 0;
int64_t inputNumber = 0;
uint8_t commaPos = 0;

bool comma = false;
bool displayResult = true;
bool hiddenMode = false;

Key operation = KEY_EQUALS;

void fatal_error()
{
    LCD::ClearScreen();
    LCD::MoveCursor(0, 0);
    LCD::WriteString("ERROR");
    while(true);
}

void print_number(uint8_t column, int64_t number)
{
    int64_t temp = number;
    if (temp < 0) temp = -temp;
    uint8_t i;
    for (i = 0; temp > 0 || i == 0; i++)
    {
        LCD::MoveCursor(15-i, column);
        LCD::WriteChar('0'+(temp%10));
        temp /= 10;
    }
    if (number < 0)
    {
        LCD::MoveCursor(15-i, column);
        LCD::WriteChar('-');
    }
}

void update_screen()
{
    LCD::ClearScreen();

    // Shows hidden mode indicator.
    if(hiddenMode){
        LCD::MoveCursor(15, 0);
        LCD::WriteChar('~');
    }

    if(displayResult)
    {
        print_number(1, result);
    }
    else
    {
        if(result != 0)
            print_number(0, result);
        print_number(1, inputNumber);
    }

    if (operation != KEY_EQUALS || displayResult)
    {
        LCD::MoveCursor(0, 1);
        LCD::WriteChar(operation);
    }
}

int main()
{
    LCD::Init();
    Keyboard::Init();
    Speaker::Init();

    Speaker::Beep(1000, 100);

    update_screen();
    while (true)
    {
        Key key = Keyboard::GetKey();

        if (key < 10)
        {
            if (displayResult)
            {
                displayResult = false;
                if(operation == KEY_EQUALS)
                {
                    result = 0;
                }
            }
            if (!comma)
            {
                inputNumber = inputNumber * 10 + key;
            }
            else
            {
                //inputNumber = inputNumber + ((int64_t)key / (10*commaPos++));
            }
        }
        else if (key == KEY_COMMA)
        {
            if (!displayResult && !comma)
            {
                comma = true;
                commaPos = 1;
            }
        }
        else if (key == KEY_PLUS_MINUS)
        {
            inputNumber = -inputNumber;
        }
        else if (key == KEY_ADD || key == KEY_SUB || key == KEY_MUL || key == KEY_DIV || key == KEY_EQUALS || key == KEY_SQRT)
        {
        	displayResult = true;

            if(!displayResult)
            {
            	// 1337 divided by 0 activates hidden mode
            	if(operation == KEY_DIV && result == 1337 && inputNumber == 0){
            		hiddenMode = true;
            		displayResult = false;

            		update_screen();
            		continue;
            	}

                switch(operation)
                {
                    case KEY_ADD: result += inputNumber; break;
                    case KEY_SUB: result -= inputNumber; break;
                    case KEY_MUL: result *= inputNumber; break;
                    case KEY_DIV: result /= inputNumber; break;
                    case KEY_EQUALS: result = inputNumber; break;
                    default: fatal_error();
                }
                inputNumber = 0;
                comma = false;
            }
            if(key == KEY_SQRT)
            {
                result = sqrt(result);
                operation = KEY_EQUALS;
            }
            else
            {
                operation = key;
            }
        }
        else if(key == KEY_ON)
        {
            //const static void (*reset)(void) __attribute__((noreturn)) = (const void(*)())0;
            cli();
            wdt_enable(WDTO_15MS);
            while (true);
        }
        else if(key == KEY_SOUND)
        {
            LCD::ToggleBacklight();
        }
        else
        {
            //fatal_error();
            LCD::MoveCursor(0, 0);
            LCD::WriteChar(key);
            _delay_ms(100);
        }

        update_screen();
    }

	return 0;
}
