// Melakukan startup guna mengatur:
// 1. Jumlah channel yang digunakan
// 2. Jeda atau delay waktu pembacaan
void settingMenu(){
  
  byte cursorPosition = 1;
  byte prevKey = 0;
  byte maxValue = 0;
  
  while(startLoop == false){
    displaySetting();
    key = getKey();
    
    switch(cursorPosition){
      case 1:
          value = constrain(value, 0, 48);
          sensor = value;
          
          lcd.setCursor(0, 0);
          lcd.write(1);
          lcd.setCursor(0, 1);
          lcd.print(" ");
          delay(250);
          if(key != prevKey){
            prevKey = key;
            if(key == 3){
                cursorPosition = 2;
                value = setDelay[0];
            }
          }
          break;
      case 2:
          value = constrain(value, 0, 59);
          setDelay[0] = value;
          
          lcd.setCursor(0, 0);
          lcd.print(" ");
          lcd.setCursor(0, 1);
          lcd.write(1);
          delay(250);
          if(key != prevKey){
            prevKey = key;
            if(key == 3){
              cursorPosition = 3;
              value = setDelay[1];
            } else if(key == 4){
              cursorPosition = 1;
              lcd.clear();
              value = sensor;
            }
          }
          break;
      case 3:
          value = constrain(value, 0, 2);
          setDelay[1] = value;
          
          lcd.setCursor(0, 1);
          lcd.print(" ");
          lcd.setCursor(15, 1);
          lcd.write(2);
          delay(250);
          if(key != prevKey){
            prevKey = key;
            if(key == 3){
              setDelay[1] = pow(60, value);
              lcd.clear();
              startLoop = true;
              pageMenu = 2;
              
              Serial.print(sensor);
              Serial.print("Channel\t Delay ");
              Serial.print(setDelay[0]);
              Serial.println(charDelay[setDelay[1] - 1]);
              
            } else if(key == 4){
              cursorPosition = 2;
              lcd.clear();
              value = setDelay[0];
            }
          }
          break;
    }
    
    if(cursorPosition >= 3){
      cursorPosition = 3;
    }
  }
}

void displaySetting(){
  lcd.setCursor(3, 0);
  lcd.print("Use :");
  lcd.setCursor(8, 0);
  lcd.print(sensor);
  lcd.print(" Ch  ");
  
  lcd.setCursor(1, 1);
  lcd.print("Delay :");
  lcd.setCursor(8, 1);
  lcd.print(setDelay[0]);
  lcd.print(" ");
  lcd.print(charDelay[setDelay[1]]);
  lcd.print("  ");
  
}

// Memulai membaca, menampilkan, dan menyimpan data
void getData(){
    
  unsigned long previousMillis = 0;
  const long interval = 1000*(setDelay[0]*setDelay[1]);
  byte counter = 0;
  while(startLoop == true)
  {
    DateTime now = RTC.now();
    timeClock(11, 0);
    
    int sensors = sensor;
    int sensorBefore = 0;
    unsigned long currentMillis = millis();
    
    if(currentMillis - previousMillis >= interval){

      digitalWrite(ledIn, HIGH);
      logfile.print('"');
      logfile.print(now.year(), DEC);
      logfile.print("/");
      logfile.print(now.month(), DEC);
      logfile.print("/");
      logfile.print(now.day(), DEC);
      logfile.print(" ");
      logfile.print('"');
      logfile.print(", ");
      
      logfile.print('"');
      logfile.print(now.hour(), DEC);
      logfile.print(":");
      logfile.print(now.minute(), DEC);
      logfile.print(":");
      logfile.print(now.second(), DEC);
      logfile.print('"');
      logfile.print(", ");
      
      for(int j = 0; j < 3; j++){
        sensors = sensors - sensorBefore;
        if((sensors > 0) && (sensors <= 48)){
          for(int i = 0; i < sensors; i++){    
            readMux(i);
            int test = analogRead(j);
            
            int addr = i + (16 * j);
            int val = 0;
            val = analogRead(j);

            logfile.print(val);
            logfile.print(", ");
            logfile.print(temp(val));
            logfile.print(", ");
            
            Serial.print(addr);
            Serial.print(": ");
            Serial.print(temp(val));
            dataList[addr] = val;
            Serial.print("\t");

            sensorBefore = i + 1;
            
            if(i >= 15){
              break;
            }
          }
          logfile.println();
          Serial.println();
        }
      }
      logfile.flush();
      previousMillis = currentMillis;
      digitalWrite(ledIn, LOW);
    }
    lcdPreview();
  }
}

