//Mic-Breakout
int mic = A0;
long micVal = 0;

void setup() {  
  Serial.begin(9600);
  pinMode(mic,INPUT);
}

void loop() {
  Serial.print(analogRead(mic));
  Serial.print("     ");
  Serial.println(getMicVal());
  delay(50);
  // Sprechen (dist 20cm)  ca. 10 - 25
  // Klatschen max ca. 120
  // Summer max ca. 350
  // Ins Mic pusten max 500 
}
/*
 * Get a signal from Mic every @intervall ms
 * Info: the method gets the micVal 'period' times and 
 * calculates the average.
 */
long getMicVal(){
  int period = 3;
  int correction_value = 510;
  for(int i = 0; i < period; i++){
    micVal = micVal + abs(analogRead(mic)-correction_value);
    delay(5);
  }
  micVal = constrain(abs(micVal/period),1,500);    
  return(micVal);
}

