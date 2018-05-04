#include <Arduino.h>

#define latchPin 2
#define clockPin 3
#define dataPin  4

#define inputPin 5

#define LED_1_RED    (1 << 0)
#define LED_1_GREEN  (1 << 2)
#define LED_1_BLUE   (1 << 1)
#define LED_2_RED    (1 << 4)
#define LED_2_GREEN  (1 << 3)
#define LED_2_BLUE   (1 << 5)

#define LED_3_RED    (1 << 8)
#define LED_3_GREEN  (1 << 10)
#define LED_3_BLUE   (1 << 9)
#define LED_4_RED    (1 << 12)
#define LED_4_GREEN  (1 << 11)
#define LED_4_BLUE   (1 << 13)


uint16_t d = 1;
int state;


void writedata(uint16_t data)
{
	// Serial.print("writing ");
	// Serial.println(data, BIN);
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, MSBFIRST, data >> 8);
    shiftOut(dataPin, clockPin, MSBFIRST, 0xFF & data);
    digitalWrite(latchPin, HIGH);
}

void reportToSerial()
{
	if (HIGH==state)
		Serial.println("CLOSED");
	else
		Serial.println("OPEN");
}

void setup()
{
	Serial.begin(9600);
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);

	pinMode(inputPin, INPUT_PULLUP);

	// determine initial state
	state = digitalRead(inputPin);
	reportToSerial();
	uint16_t d;
	if (state == HIGH)
		d = LED_1_RED | LED_2_RED | LED_3_RED | LED_4_RED;
	else
		d = LED_1_GREEN | LED_2_GREEN | LED_3_GREEN | LED_4_GREEN;

	// flicker a few times
	for (int i=0;i<5;i++)
	{
		writedata(d);
		delay(10);
		writedata(0);
		delay(50+random(100));
	}
	for (int i=0;i<5;i++)
	{
		writedata(d);
		delay(50+random(200));
		writedata(0);
		delay(50);
	}
	delay(500);
	writedata(d);
}



void loop() {
	delay(500);
	int newstate = digitalRead(inputPin);
	if (newstate != state)
	{
		reportToSerial();
		if (newstate == LOW)
		{
			writedata(LED_1_GREEN | LED_2_RED   | LED_3_RED   | LED_4_RED);
			delay(500);
			writedata(LED_1_GREEN | LED_2_GREEN | LED_3_RED   | LED_4_RED);
			delay(200);
			writedata(LED_1_GREEN | LED_2_GREEN | LED_3_GREEN | LED_4_RED);
			delay(100);
			writedata(LED_1_GREEN | LED_2_GREEN | LED_3_GREEN | LED_4_GREEN);
		}
		else
		{
			writedata(LED_1_RED | LED_2_GREEN   | LED_3_GREEN   | LED_4_GREEN);
			delay(500);
			writedata(LED_1_RED | LED_2_RED | LED_3_GREEN   | LED_4_GREEN);
			delay(200);
			writedata(LED_1_RED | LED_2_RED | LED_3_RED | LED_4_GREEN);
			delay(100);
			writedata(LED_1_RED | LED_2_RED | LED_3_RED | LED_4_RED);
		}
	}
	state = newstate;



	// Serial.println("fast blink");
	// for (int i=0; i<10;i++){
	// 	writedata(16);
	// 	delay(50+random(200));
	// 	writedata(8);
	// 	delay(50+random(1000));
	// }
	// Serial.println("med blink");
	// for (int i=0; i<10;i++){
	// 	writedata(16);
	// 	delay(50+random(200));
	// 	writedata(8);
	// 	delay(50+random(200));
	// }
	// Serial.println("inv blink");
	// for (int i=0; i<10;i++){
	// 	writedata(16);
	// 	delay(50+random(1000));
	// 	writedata(8);
	// 	delay(50+random(200));
	// }
	// d = d << 1;
	// if (!d) d = 1;
	//
	// writedata(d);
	// delay(2000);

}
