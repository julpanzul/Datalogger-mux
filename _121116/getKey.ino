// Mendefinisikan setiap tombol dan nilai resistansinya
// Gunakan Analog teset untuk setting ulang akurasi tombol

char getKey(){
  int val = 0;
  byte button = 0;
  val = analogRead(keypadPin);
  
  button = NO_KEY;
  if (val <= 290) {button = A; value++;}
  else if((val >= 300) && (val <= 400)) {button = B; value--;}
  else if((val >= 410) && (val <= 500)) {button = C; }
  else if((val >= 510) && (val <= 600)) {button = D; }
  return button;
}
