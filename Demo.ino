// Inisiasi LCD
// Sebelum compile pastikan sudah menginstall library LCD atau download di https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
#include <LCD.h>
#include <LiquidCrystal_SR.h>

LiquidCrystal_SR lcd(2, 4, 3); // LCD menggunakan pin 2, 4, 3

//Pin kontrol (Digital) 5, 6, 7, 8
int s0 = 8;
int s1 = 7;
int s2 = 6;
int s3 = 5;

//kriteria pada array
int myMux[] = {1, 3, 1};

void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);
  
  pinMode(s0, OUTPUT); 
  pinMode(s1, OUTPUT); 
  pinMode(s2, OUTPUT); 
  pinMode(s3, OUTPUT); 

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);

}


void loop(){
  //Mengambil nilai tiap channel
  /*   
  Serial.println("Logika merubah posisi channel dan pin cara tunggal:");
  
  for(int y=0; y<=channel; y++){
    Serial.print("Input channel ");
    Serial.println(y);
    for(int z=0; z<=pin; z++){
      Serial.print("\tpin ");
      Serial.println(z);
      delay(1000);
    }
  }
*/
  Serial.println("Logika merubah posisi pin cara multiple:");
  
  for(int j=0; j<=myMux[1]; j++){
    Serial.print("pin ");
    Serial.print(j);
    Serial.print(": ");
    lcd.setCursor(0, 0);
    lcd.print(j);
    for(int i=0; i<=myMux[0]; i++){
      readMux(j);
      Serial.print(analogRead(i), DEC);
      Serial.print("\t");
      lcd.print("|");
      lcd.print(analogRead(i), DEC);
    }
    lcd.print("    ");
    Serial.println();
    delay(myMux[2] * 1000);
  }
}

//Me-multiplex 4 pin menjadi 16 channel
int readMux(int channel){
  int controlPin[] = {s0, s1, s2, s3};

  int muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };

  for(int i = 0; i < myMux[1]; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
}
