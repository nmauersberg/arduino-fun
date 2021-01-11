#include <DS3231.h>
#include <TM1637Display.h>

RTClib RTC;

#define CLK 2
#define DIO 3

TM1637Display display = TM1637Display(CLK, DIO);

const uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
const uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };

int pin = 6;
int blue = 12;
int green = 11;
int red = 10;

int prevSecond = 0;

int digits [8] = { 0, 0, 0, 0, 0, 0, 0, 0 }; 

void setCounter(int n) {
 for (int i=0; i <= 7; i++) {
  digits[i] = bitRead(n, i);
 }
}

void fireTick (int pin, int color, int delayMs) {
  printTimestamp();
  digitalWrite(pin, HIGH);
  digitalWrite(color, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  delay(delayMs);
  digitalWrite(pin, LOW);
  digitalWrite(color, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
}

void fireMarker (int pin, int primaryColor, int secondaryColor, int delayMs) {
  digitalWrite(pin, HIGH);
  digitalWrite(primaryColor, HIGH);
  digitalWrite(secondaryColor, HIGH);
  delay(delayMs);
  digitalWrite(pin, LOW);
  digitalWrite(primaryColor, LOW);
  digitalWrite(secondaryColor, LOW);
  delay(delayMs * 2);
}

void reactToDigit (bool value, int pin, int color, int delayMs) {
  if (value) {
    Serial.print("1");
    digitalWrite(pin, HIGH);
    digitalWrite(color, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    delay(delayMs);
    digitalWrite(pin, LOW);
    digitalWrite(color, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delayMs);
  } else {
    Serial.print("0");
    delay(delayMs * 2);
  }
}

void printTimestamp() {
  DateTime now = RTC.now();
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

void setup() {
  Serial.begin(57600);
  display.clear();
  pinMode(pin, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  DateTime now = RTC.now();
  int currHour = now.hour();
  int currMinute = now.minute();
  int currSecond = now.second();

  display.setBrightness(2);

  
  
  if(currSecond != prevSecond) {
    now = RTC.now();
    prevSecond = now.second();

    int currHourMin = (currHour * 100) + (currMinute % 100);
    display.showNumberDecEx(currHourMin, 0b01000000, false, 4, 0);

    if (currSecond % 10 != 0 && currSecond % 60 != 0) {  
     fireTick(pin, blue, 30);

     // int currMinSec = (currMinute * 100) + (currSecond % 100);
     // display.showNumberDecEx(currMinSec, 0b01000000, false, 4, 0);

     display.showNumberDec(currSecond);

     delay(500);

     int currHourMin = (currHour * 100) + (currMinute % 100);
      display.showNumberDecEx(currHourMin, 0b01000000, false, 4, 0);
     
    } else if (currSecond % 60 != 0) {
      printTimestamp();
      
      setCounter(currMinute);  
       
      Serial.print("Minute: ");
      Serial.print(currMinute);
      Serial.print(" in binary: ");
      
      fireMarker(pin, blue, green, 75);
      for (int i=0; i<=7; i++) {
        reactToDigit(digits[i], pin, green, 50);
      }
      
      Serial.println();
    } else {
      printTimestamp();
      
      setCounter(currHour);

      int currHourMin = (currHour * 100) + (currMinute % 100);
      display.showNumberDecEx(currHourMin, 0b01000000, false, 4, 0);

      Serial.print("Hour: ");
      Serial.print(currHour);
      Serial.print(" in binary: ");

      fireMarker(pin, blue, red, 75);

      for (int i=0; i<=7; i++) {
        reactToDigit(digits[i], pin, red, 50);
      }
      
      Serial.println();
    }
  }
}
