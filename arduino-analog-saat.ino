/* arduino-analog-saat.ino
 *
 * By: Evren EFE
 * Processor: ATmega328P
 * Compiler:  Arduino AVR
 */
#include <avr/pgmspace.h>
#include <DS3231.h>
#include <Wire.h>

//Dakika
//Pin connected to ST_CP of 74HC595
#define latchPin 5
//Pin connected to SH_CP of 74HC595
#define clockPin 3
//Pin connected to DS of 74HC595
#define dataPin 4
 
DS3231 Clock;


//Saat ledleri matrix
const byte saatler[12] PROGMEM = {
 //KKKKAAAA
   B01110100,  // 0-12
   B11100001,  // 1
   B11010001,  // 2
   B10110001,  // 3
   B01110001,  // 4
   B11100010,  // 5
   B11010010,  // 6
   B10110010,  // 7
   B01110010,  // 8
   B11100100,  // 9
   B11010100,  // 10
   B10110100  // 11
}; //kod yazmaya erindim

unsigned long previousMillis = 0;
unsigned long interval = 1000;
int sa = 1;
int da = 0;
int sn = 0;
boolean clockExist = false;
boolean oscOk = false;

void setup()
 { 
   Serial.begin(9600);
   pinMode(dataPin, OUTPUT);
   pinMode(clockPin, OUTPUT);
   pinMode(latchPin, OUTPUT);
  
   //Check Clock Device
   Wire.begin();
   Wire.beginTransmission(0x68);
   if (Wire.endTransmission() == 0){
      clockExist = true;
   }
   
   
   // Checks the status of the OSF (Oscillator Stop Flag);.
   if(clockExist) {
      oscOk = Clock.oscillatorCheck();
      Serial.println(F("RTC OK"));
   }else{
      Serial.println(F("NO RTC"));
   }
   if (!oscOk) {
      Serial.println(F("OSF Set"));
      interval = 10;
   }
}
 
void ReadDS3231()
{
  bool h12;
  bool PM;
  sn=Clock.getSecond();
  da=Clock.getMinute();
  sa=Clock.getHour(h12, PM);
  sa = sa % 12;
  Serial.print(sa,DEC);
  Serial.print(':');
  Serial.print(da,DEC);
  Serial.print(':');
  Serial.println(sn);
}

void Demo() {
   sn =0;
   sa = sa + 1;
   da = da + 1;
   sa = sa % 12;
   da = da % 60;
   interval = interval + 50;
   interval = interval % 500;
}

void loop()
{
 unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
   previousMillis = currentMillis;
   if(oscOk) {
      ReadDS3231();
   }else{
      Demo();
   }
   kadranGuncelle();
  } 
}

void kadranGuncelle(){
 byte ua = 0;
 byte uk = 0;
 byte a = 0;
 byte k = 0;

 a = da / 8;
 k = da % 8;
 ua = 1 << a;
 uk = B11111111 ^ ( 1 << k );

 digitalWrite(latchPin, LOW);
 shiftOut(dataPin, clockPin, MSBFIRST, pgm_read_byte_near(saatler + sa)); 
 shiftOut(dataPin, clockPin, MSBFIRST, ua); 
 shiftOut(dataPin, clockPin, MSBFIRST, uk); 
 digitalWrite(latchPin, HIGH);
}

