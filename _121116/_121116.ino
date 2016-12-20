#include <Wire.h>                    // Liquid Crystal 16x2
#include <LCD.h>
#include <LiquidCrystal_SR.h>
LiquidCrystal_SR lcd(2, 4, 3);

// icon tanda panah
byte arrow[8] = {
  0b10000,
  0b11000,
  0b11100,
  0b11110,
  0b11100,
  0b11000,
  0b10000,
  0b00000
};

// icon tanda panah terbalik
byte backarrow[8] = {
  0b00001,
  0b00011,
  0b00111,
  0b01111,
  0b00111,
  0b00011,
  0b00001,
  0b00000
};

// icon suhu
byte IconTemp[8] = {
  0b00100,
  0b01010,
  0b01010,
  0b01010,
  0b01110,
  0b11111,
  0b11111,
  0b01110
};

// icon kelembaban
byte IconHumid[8] = {
  0b00100,
  0b00100,
  0b01010,
  0b01010,
  0b10001,
  0b10001,
  0b10001,
  0b01110
};

byte berhenti[8] = {
  0b00000,
  0b00000,
  0b11110,
  0b11110,
  0b11110,
  0b11110,
  0b00000,
  0b00000
};

#include "RTClib.h"

RTC_DS1307 RTC;
char* days[] = 
{
  "Ahad", 
  "Sen ", 
  "Sel ", 
  "Rab ", 
  "Kam ", 
  "Jum ", 
  "Sab "
};
char* months[] = 
{
  "Jan", 
  "Feb", 
  "Mar", 
  "Apr", 
  "Mei", 
  "Jun", 
  "Jul", 
  "Aug", 
  "Sep", 
  "Okt", 
  "Nov", 
  "Des"
};

#include <SPI.h>                      // SD Card
#include <SD.h>

// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;
 
// the logging file
File logfile;

/*
The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 10
*/

int keypadPin = A3;                   // Keypad Controllers
const byte NO_KEY = 0;
const byte A = 1;
const byte B = 2;
const byte C = 3;
const byte D = 4;
byte key = 0;

int ledIn = 9;                        // Lampu indikator

int s0 = 8;                           // Mux control pins
int s1 = 7;
int s2 = 6;
int s3 = 5;

//Mux in "SIG" pin

byte value = 0;
byte pageMenu = 1;
boolean startLoop = false;
int sensor = 0;
unsigned long setDelay[] = {0, 0};
char* charDelay[] = {"sec ", "min ", "hour"};

byte dataList[48];

void setup ()
{
  Serial.begin(9600);
  lcd.begin(16, 2);

  lcd.createChar(1, arrow);
  lcd.createChar(2, backarrow);
  lcd.createChar(3, IconTemp);
  lcd.createChar(4, IconHumid);

  
  // initialize the SD card
  Serial.print("Initializing SD card...");
  lcd.setCursor(0, 0);
  lcd.print("Reading Card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    error("Card failed");
  } else {
    Serial.println("card initialized.");
    success("Card success");
  }
  
  // create a new file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }
  
  if (! logfile) {
    error("Can't create file");
    lcd.clear();
  } else {
    Serial.print("Logging to: ");
    lcd.setCursor(0, 0);
    lcd.print("File name:");
    success(filename);
  }

  // connect to RTC
  Wire.begin();  
  if (!RTC.begin()) {
    logfile.println("RTC failed");
    Serial.println("RTC failed");
  }
  
  pinMode(keypadPin, INPUT_PULLUP);
  pinMode(s0, OUTPUT); 
  pinMode(s1, OUTPUT); 
  pinMode(s2, OUTPUT); 
  pinMode(s3, OUTPUT);
  pinMode(ledIn, OUTPUT); 

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  
  logfile.println("millis,stamp,datetime,light,temp,vcc");  
}

void loop () {
  DateTime now = RTC.now();
  
  switch(pageMenu){
    case 1:
      settingMenu();
      break;
    case 2:
      getData();
      break;
  }

  if((pageMenu > 2)&&(pageMenu < 1)){
    pageMenu = 1;
  }
}
