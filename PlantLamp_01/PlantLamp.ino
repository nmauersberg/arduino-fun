#include <Adafruit_NeoPixel.h>
#include "RTClib.h"
#include <Wire.h>

#define LED_PIN 3
#define LED_COUNT 24

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup()
{
  Serial.begin(57600);

  pinMode(LED_PIN, OUTPUT);
  strip.begin();
  strip.show();

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
}

void loop()
{
  DateTime now = rtc.now();

  int currHour = now.hour();

  Serial.println(currHour);

  if (currHour >= 7 && currHour <= 17)
  {
    for (int i = 0; i < LED_COUNT; i++)
    {
      strip.setPixelColor(i, strip.Color(255, 140, 140));
      strip.show();
      delay(50);
    }
    delay(59000);
  }
  else
  {
    for (int i = 0; i < LED_COUNT; i++)
    {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
      strip.show();
      delay(50);
    }
    delay(60000);
  }
}
