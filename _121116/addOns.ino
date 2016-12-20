// Fungsi tambahan

//Menampilkan digit '0' didepan angka tunggal
void addDigits(int numar)
{
    if((numar >= 0) && (numar <= 9)){
      lcd.write('0');
    }
    lcd.print(numar);
}

//Mengkonversi nilai resistansi menjadi 'Celcius'
float temp(int val){
  float mv = ( val/1024.0)*5000; 
  float cel = mv/10;

  return cel;
}

//Mengkonversi nilai resistansi menjadi '%'
float percent(int val){
  float value = map(val, 0, 1024, 0, 100); //map(min_input, max_input, min_output, max_output)
  return value;
}

//Menampilkan pesan error
void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
  lcd.setCursor(0, 1);
  lcd.print(str);
  delay(5000);
}

//Menampilkan pesan sukses
void success(char *str){
  Serial.println(str);
  lcd.setCursor(0, 1);
  lcd.print(str);
  delay(5000);
  lcd.clear();
}

//RTC untuk membaca waktu
void timeClock(int x, int y){
  DateTime now = RTC.now();
  
  lcd.setCursor(x, y);
  addDigits(now.hour());
  if(now.second() % 2 == 0){
    lcd.print(':');
  } else {
    lcd.print(' ');
  }
  addDigits(now.minute());
}

//RTC untuk tanggal
void timeDate(){
  DateTime now = RTC.now();
  
  lcd.setCursor(1, 1);
  lcd.print(days[now.dayOfTheWeek()]);
  
  addDigits(now.day());
  lcd.print("/");
  addDigits(now.month());
  lcd.print("/");
  lcd.print(now.year());
}

// Disini akan diberi switch case untuk mengubah output jadi suhu atau kelembaban
// icon suhu kelembaban

byte viewMode = 1;

void lcdPreview(){
  key = getKey();
  value = constrain(value, 0, sensor-1);
  
  byte prevKey;
  if(key != prevKey){
    prevKey = key;
    if(key == 3){
      viewMode++;
      lcd.clear();
    }
  }
  switch(viewMode){
    case 1:
      lcd.setCursor(0, 1);
      lcd.write(3);
      lcd.print(" ");
      lcd.print(temp(dataList[value]));
      lcd.print("oC    ");
      break;
    case 2:
      lcd.setCursor(0, 1);
      lcd.write(4);
      lcd.print(" ");
      lcd.print(percent(dataList[value]));
      lcd.print("m3/m3 ");
      break;
      
  }
  if(viewMode > 2){viewMode = 1;}
  
  lcd.setCursor(0, 0);
  lcd.print("Ch ");
  lcd.print(value);
  lcd.print(": ");
  lcd.setCursor(6, 0);
  lcd.print(dataList[value]);
  lcd.print("mV ");

  delay(200);
}

