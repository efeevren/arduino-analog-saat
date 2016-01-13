/* Main.ino
 * Coder: Evren EFE
 * Created: 26/11/2015
 * Processor: ATmega328P
 * Compiler:  Arduino AVR
 * Description: 3 adet 74HC595 ile anolog görünümlü saat projesi
 *
 * Version History:
 * 0.1 - Temel multiplexing yapıldı.
 * 0.2 - Multiplexing düzeltildi, DS3231'den saat okuma eklendi
 */

#include <DS3231.h>
#include <Wire.h>
 
DS3231 Clock;

//Dakika
//Pin connected to ST_CP of 74HC595
const byte latchPin = 5;
//Pin connected to SH_CP of 74HC595
const byte clockPin = 3;
//Pin connected to DS of 74HC595
const byte dataPin = 4;

//Saat ledleri matrix
byte saatler[13] = {
 //KKKKAAAA
   B11110000,  // 0-none
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
   B10110100,  // 11
   B01110100  // 12
}; //kod yazmaya erindim

unsigned long previousMillis = 0;
const long interval = 1000;
byte sa = 1;
byte da = 0;
byte sn = 0;


void setup()
 { 
  Serial.begin(9600);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  

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
 
void loop()
{
 unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
   previousMillis = currentMillis; 
   ReadDS3231();
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
 shiftOut(dataPin, clockPin, MSBFIRST, saatler[sa]); 
 shiftOut(dataPin, clockPin, MSBFIRST, ua); 
 shiftOut(dataPin, clockPin, MSBFIRST, uk); 
 digitalWrite(latchPin, HIGH);
}

