#!/bin/bash
set -e
avr-gcc -Wall -Os -mmcu=atmega8 -std=c++11 -DF_CPU=1000000UL main.cpp lcd.cpp keyboard.cpp speaker.cpp math.cpp -o main.elf
avr-objcopy -j .text -j .data -O ihex main.elf main.hex
avr-size --format=avr --mcu=atmega8 main.elf
sudo avrdude -c usbasp -p m8 -B3 -U flash:w:main.hex:i
