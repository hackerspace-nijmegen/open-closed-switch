Arduino code is tested on arduino nano (atmega328P). I used PlatformIO, which is can sincerely recommend but if you don't want to use that, the code in main.cpp is fully arduino compatible and can be copied and pasted into the arduino IDE. I did not use any libraries so that should work without further configuration.

The current code "flickers" the leds a few times on boot in de color of the current switch status. Whenever the switch is changed the leds change one-by-one. The animation is used as a crude debounce, hence no further debouncing is implemented. 

The 12V (non-addressable/dumb) RGB led strips are attached via two daisy-chained boards, each of which contains a 74HC595 shift register and 6 mosfest that each control one channel. Hence the colors are written using shiftOut() and not digitalWrite(). 

Since my simple animations only requires full red and full green I have not used any kind of pwm. Full color/fading can be achieved by using ShiftPWM library. I've used that before with the exact same boards and works great. It may be necessary to change the pins to SPI for faster io but with only two shift registers that may not be necessary. For full-color control the sketch will have to go major surgery.
