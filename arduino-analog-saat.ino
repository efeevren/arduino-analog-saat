/* Main.ino
 * Coder: Evren EFE
 * Created: 26/11/2015
 * Processor: ATmega328P
 * Compiler:  Arduino AVR
 * Description: 3 adet 74HC595 ile anolog görünümlü saat projesi
 *
 * Version History:
 * 0.1 - Temel multiplexing yapıldı.
 */

//Dakika
//Pin connected to ST_CP of 74HC595
const byte latchPin = 5;
//Pin connected to SH_CP of 74HC595
const byte clockPin = 3;
//Pin connected to DS of 74HC595
const byte dataPin = 4;

//Saat
//Pin connected to ST_CP of 74HC595
const byte slatchPin = 8;
//Pin connected to SH_CP of 74HC595
const byte sclockPin = 6;
//Pin connected to DS of 74HC595
const byte sdataPin = 7;


unsigned long dpreviousMillis = 0;
unsigned long spreviousMillis = 0;
const long dinterval = 100;
const long sinterval = 4500;
byte sa = 1;
byte da = 0;
void setup()
 { 
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(sdataPin, OUTPUT);
  pinMode(sclockPin, OUTPUT);
  pinMode(slatchPin, OUTPUT);
  
/*
  digitalWrite(latchPin, LOW);
  //U_A
  shiftOut(dataPin, clockPin, MSBFIRST, B00000000); 

  //U_K
  shiftOut(dataPin, clockPin, MSBFIRST, B11111111); 
  digitalWrite(latchPin, HIGH);
*/
  saat(sa);
  dakika(da);
 }

void loop()
{
 unsigned long currentMillis = millis();
  if(currentMillis - dpreviousMillis >= dinterval) {
    dpreviousMillis = currentMillis; 
    da++;
    if (da >= 60) { da = 0; }
    dakika(da);
  } 
 if(currentMillis - spreviousMillis >= sinterval) {
    spreviousMillis = currentMillis; 
    sa++;
    if (sa >= 13) { sa = 1; }
    saat(sa);
  }
}

void dakika(byte dk) {
 byte ua = 0;
 byte uk = 0;
 byte a = 0;
 byte k = 0;

 a = dk / 8;
 k = dk % 8;
 ua = 1 << a;
 uk = B11111111 ^ ( 1 << k );
 digitalWrite(latchPin, LOW);
 shiftOut(dataPin, clockPin, MSBFIRST, ua); 
 shiftOut(dataPin, clockPin, MSBFIRST, uk); 
 digitalWrite(latchPin, HIGH);
}

void saat(byte sa) {
 byte saatler[13] = {
  B00001111,  // 0-none
  B10000111,  // 1
  B10001011,  // 2
  B10001101,  // 3
  B10001110,  // 4
  B01000111,  // 5
  B01001011,  // 6
  B01001101,  // 7
  B01001110,  // 8
  B00100111,  // 9
  B00101011,  // 10
  B00101101,  // 11
  B00101110   // 12
 }; //kod yazmaya erindim
 digitalWrite(slatchPin, LOW);
 shiftOut(sdataPin, sclockPin, LSBFIRST, saatler[sa]); 
 digitalWrite(slatchPin, HIGH);
}

