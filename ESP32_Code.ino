#include <Arduino.h>

int DATA = 4;
int CLOCK = 5;
int LATCH = 6;
int OE = 7;
int BUTTON = 3;

int mode = 0;
int brightness = 0;
int step = 5;
int led = 0;
#include <Arduino.h>

int DATA = 4;
int CLOCK = 5;
int LATCH = 6;
int OE = 7;
int BUTTON = 3;

int mode = 0;
int brightness = 0;
int step = 5;
int led = 0;

unsigned long timer = 0;

void sendLEDs(unsigned long value)
{
  digitalWrite(LATCH, LOW);

  for (int i = 3; i >= 0; i--)
  {
    byte data = (value >> (i * 8)) & 255;
    shiftOut(DATA, CLOCK, MSBFIRST, data);
  }

  digitalWrite(LATCH, HIGH);
}

void setBrightness(int value)
{
  ledcWrite(OE, 255 - value);
}

void setup()
{
  pinMode(DATA, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  ledcAttach(OE, 5000, 8);

  sendLEDs(0);
  setBrightness(0);
}

void loop()
{
  if (digitalRead(BUTTON) == LOW)
  {
    mode++;

    if (mode > 2)
    {
      mode = 0;
    }

    led = 0;
    brightness = 0;

    delay(250);
  }

  if (mode == 0)
  {
    sendLEDs(134217727);
    setBrightness(255);
  }

  if (mode == 1)
  {
    if (millis() - timer > 20)
    {
      brightness = brightness + step;

      if (brightness >= 255)
      {
        brightness = 255;
        step = -5;
      }

      if (brightness <= 10)
      {
        brightness = 10;
        step = 5;
      }

      timer = millis();
    }

    sendLEDs(134217727);
    setBrightness(brightness);
  }

  if (mode == 2)
  {
    if (millis() - timer > 100)
    {
      sendLEDs(1UL << led);

      led++;

      if (led >= 27)
      {
        led = 0;
      }

      timer = millis();
    }

    setBrightness(255);
  }
}