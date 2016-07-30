//Pin kontrol (Digital)
int s0 = 11;
int s1 = 10;
int s2 = 9;
int s3 = 8;

//Pin Input (Analog)
int SIG_1 = 1;
int SIG_2 = 2;

int x = 0;

void setup(){
  Serial.begin(9600);
  
  pinMode(s0, OUTPUT); 
  pinMode(s1, OUTPUT); 
  pinMode(s2, OUTPUT); 
  pinMode(s3, OUTPUT); 

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  
  Serial.println("Berapa channel yang akan digunakan?");
}


void loop(){
  if(Serial.available()){
    char i = Serial.read();
    if(i>='0' && i<='9'){
      x = i - '0';
    }
  }
  //Mengambil nilai tiap channel
  //i = posisi channel
  if(x > 0){
    for(int i = 0; i < x; i ++){
      Serial.print("Value at channel ");
      Serial.print(i);
      Serial.print("is : ");
      readMux(i);
      Serial.print(analogRead(SIG_1), DEC);
      Serial.print("\t");
      Serial.println(analogRead(SIG_2), DEC);
      delay(1000);
    }
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

  for(int i = 0; i < 16; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
}
