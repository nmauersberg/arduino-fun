// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int vibrPinOne = 6;
int vibrPinTwo = 7;
int vibrPinThree = 8;
int vibrPinFour = 9;

int prevSecond = 0;

int digits[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void setCounter(int n)
{
  for (int i = 0; i <= 7; i++)
  {
    digits[i] = bitRead(n, i);
  }
}

void printTimestamp()
{
  DateTime now = rtc.now();
  Serial.print(now.day(), DEC);
  Serial.print('.');
  Serial.print(now.month(), DEC);
  Serial.print('.');
  Serial.print(now.year(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
}

void fireTick(int pin, int delayMs)
{
  printTimestamp();
  digitalWrite(pin, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  delay(delayMs);
  digitalWrite(pin, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(delayMs);
}

void reactToDigit(bool value, int delayMs)
{
  if (value)
  {

    digitalWrite(vibrPinOne, HIGH);
    digitalWrite(vibrPinTwo, HIGH);
    digitalWrite(vibrPinThree, HIGH);
    digitalWrite(vibrPinFour, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    delay(delayMs);

    digitalWrite(vibrPinOne, LOW);
    digitalWrite(vibrPinTwo, LOW);
    digitalWrite(vibrPinThree, LOW);
    digitalWrite(vibrPinFour, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delayMs);
  }
  else
  {
    delay(delayMs * 2);
  }
}

void setup()
{
  Serial.begin(57600);

#ifndef ESP8266
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB
#endif

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower())
  {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(vibrPinOne, OUTPUT);
  pinMode(vibrPinTwo, OUTPUT);
  pinMode(vibrPinThree, OUTPUT);
  pinMode(vibrPinFour, OUTPUT);
}

void loop()
{
  DateTime now = rtc.now();

  int currHour = now.hour();
  int currMinute = now.minute();
  int currSecond = now.second();

  if (currSecond != prevSecond)
  {
    now = rtc.now();
    prevSecond = now.second();

    int currHour = now.hour();
    int currMinute = now.minute();
    int currSecond = now.second();

    if (currSecond % 10 != 0 && currSecond % 60 != 0)
    {
      fireTick(vibrPinOne, 30);
      fireTick(vibrPinTwo, 30);
      fireTick(vibrPinThree, 30);
      fireTick(vibrPinFour, 30);
    }
    else if (currSecond % 60 != 0)
    {
      setCounter(currMinute);
      for (int i = 0; i <= 7; i++)
      {
        reactToDigit(digits[i], 50);
      }
    }
    else
    {
      setCounter(currHour);
      for (int i = 0; i <= 7; i++)
      {
        reactToDigit(digits[i], 50);
      }
    }
  }
}
