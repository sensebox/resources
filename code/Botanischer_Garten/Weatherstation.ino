
// Wetterstation Explorado

#include <Wire.h> // Libraries für BMP 180



int windGeschw;
float h;
float t;
unsigned long windGeschwMessIntervall = 1000; // alle 30 Sekunden startet die Messroutine
int windGeschwMessZeit = 5000; // für einen Zeitraum von 15 Sekunden
unsigned long windGeschwStart;
unsigned long windGeschwStartAlt = 0;
unsigned long windGeschwMessStart;
unsigned long windGeschwMessStartAlt = 0;

int beaufort = 0; // Windstaerke in Beaufort

const int  windGeschwPin = 9; // Kontakt/Schalter an Pin 6 digital

int windGeschwPinZaehler = 0; // Zaehler fuer die Impulse
int windGeschwPinStatus = 0; // aktueller Status des Eingangs-Pins
int windGeschwPinStatusAlt = 0; // vorheriger/alter Status des Eingangs-Pins
String windGeschwStr = "";

int windrichtungsPin = A3; // Windrichtung an Pin 3 analog
int sensorWertNeu = 0; // speichert aktuellen Wert vom Windrichtungs-Sensor
int sensorVolt1 = 0; // speichert berechneten Wert vom Windrichtungs-Sensor in Volt
int textGreifer = 4; // holt 4 Buchstaben aus Text-Array fuer Windrichtungen

char windText1; // erster Buchstabe fuer Windrichtung
char windText2; // zweiter Buchstabe fuer Windrichtung
char windText3; // dritter Buchstabe fuer Windrichtung
char windText4; // vierter Buchstabe fuer Windrichtung

// rain
int rainPin = 2;
boolean lastButtonRain = LOW;
boolean currentButtonRain = LOW;
int rainCounter=0;

#include <SHT1x.h>

// Specify data and clock connections and instantiate SHT1x object
#define dataPin  3
#define clockPin 4
SHT1x sht1x(dataPin, clockPin);

#include <BMP280.h>
BMP280 bmp;

#include <Makerblog_TSL45315.h>
Makerblog_TSL45315 luxsensor = Makerblog_TSL45315(TSL45315_TIME_M4);

#define I2C_ADDR_UV 0x38
// Integrationszeit
#define IT_1_2 0x0 //1/2T
#define IT_1   0x1 //1T
#define IT_2   0x2 //2T
#define IT_4   0x3 //4T

// Referenzwert: 0,01 W/m2 ist äquivalent zu 0.4 als UV-Index
float refVal = 0.4;

// Achtung: Freizeichen beachten, Text fuer jede Windrichtung muss genau 4 Zeichen lang sein
// Hilfestellung  :          x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   x   
//char windrichtungText1[] = {"O-SONO-OO   SO-SSO  S-SWS   N-NONO  SW-WSW  NW-NN   W-NWW   NW  "};
char windrichtungText1[] = {"113 68  90  158 135 203 180 23  45  248 225 338 0   292 270 315 "};


// Werte laut Datenblatt, aufsteigend zur Suche geordnet
int windrichtungVolt[] = {320, 410, 450, 620, 900, 1190, 1400, 1980, 2250, 2930, 3080, 3430, 3840, 4040, 4620, 4780};

// Werte in Grad laut Datenblatt, momentan noch nicht genutzt,
//int windRichtungGrad[] = {113, 68, 90, 158, 135, 203, 180, 23, 45, 248, 225, 338, 0, 292, 270, 315};

unsigned long windrichtungVoltSumme = 0; // 

long intervall = 1000; // eine Messung pro Minute (60.000 ms)
int zaehler1 = 0;
long zeit1Neu;
long zeit1Alt = 0;

void setup(){
  Serial.begin(9600);
  pinMode(windGeschwPin, INPUT); // definieren Pin 6 digital als Eingang
  digitalWrite(windGeschwPin, HIGH); // Standard: wenn Schalter offen = High    

  Serial.begin(9600);
  if (!bmp.begin()) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
  bmp.setOversampling(4);

  luxsensor.begin();

  Wire.begin();
  Wire.beginTransmission(I2C_ADDR_UV);
  Wire.write((IT_1<<2) | 0x02);
  Wire.endTransmission();
  delay(500);

  attachInterrupt(digitalPinToInterrupt(2), readRain, RISING);

}

void loop (){
 windGeschwStart = millis(); // aktualisieren Startzeit
   if (windGeschwStart - windGeschwStartAlt >= windGeschwMessIntervall) // wenn Messintervall ueberschritten ...
   {
      windgeschwmess(); // rufen wir Funktion/Subroutine zur Geschwindigkeitsmessung auf
      windGeschwStartAlt = millis(); // merken uns Startzeit der letzten Messung
   }   
 zeit1Neu = millis(); // aktualisieren Variable 
  if ((zeit1Neu - zeit1Alt) >= intervall) // Abfrage-Intervall ueberschritten?, wenn ja ...
  {  
     sensorWertNeu = analogRead(windrichtungsPin); // lesen Sensor an Pin 3 analog 
     sensorVolt1 = map(sensorWertNeu, 0, 1024, 0, 4780); // wandeln Werte aus den Bereich 0 - 1024 in 0 bis 5000 um
     if(sensorVolt1 == 4140) {
       sensorVolt1 = 4780;
     }
     windrichtungVoltSumme = windrichtungVoltSumme + sensorVolt1; // bilden Gesamtsumme
     /* nur zur Kontrolle     
     Serial.print("Messung: ");
     Serial.print(zaehler1);
     Serial.print(" SensorWertNeu: ");
     Serial.print(sensorWertNeu);
     Serial.print(" SensorVolt1 (gemapt): ");
     Serial.print(sensorVolt1);
     Serial.print(" WindrichtungVoltSumme ");
     Serial.println(windrichtungVoltSumme);
     */
     if (zaehler1 == 14) // wenn (0 bis 14 =) 15 Werte gelesen ...
     {
       windrichtungVoltSumme = windrichtungVoltSumme / 15; // bilden Durschnitt
       for (int zaehler3 = 0; zaehler3 <= 15; zaehler3++) // durchlaufen Array fuer Werte in Volt ...bis Bedingung erfüllt
       { 
           if (windrichtungVoltSumme <= windrichtungVolt[zaehler3]) // bis richtiger Wert gefunden
           {
              textGreifer = (zaehler3 * 4); // holen uns die entsprechen 4 Buchstaben fuer Text Windrichtung
              windText1 = windrichtungText1[textGreifer]; // hole 1. Buchstaben
              windText2 = windrichtungText1[textGreifer + 1]; // hole 2. Buchstaben
              windText3 = windrichtungText1[textGreifer + 2]; // hole 3. Buchstaben
              windText4 = windrichtungText1[textGreifer + 3]; // hole 4. Buchstaben
           
              //Serial.print("Mittelwert: ");
              //Serial.print(windrichtungVoltSumme);
              //Serial.print(" Windrichtung: ");
              //Serial.print(windText1);
              //Serial.print(windText2);
              //Serial.print(windText3);
              //Serial.println(windText4);
              printMeasurement();
           
             break;  // verlassen Subroutine 
           } 
       }
     windrichtungVoltSumme = 0; // setzen Variable fuer naechsten Durchlauf zurueck
     zaehler1 = -1; // setzen Variable fuer naechsten Durchlauf zurueck
     }
     zaehler1++; // erhoehen Schleifenzaehler
     zeit1Alt = zeit1Neu; // setzten verstrichenen Zeitraum auf 0
  }  
 
}
 
void windgeschwmess()
{   
   windGeschwMessStart = millis(); // aktualieren Startzeit fuer Messung
   windGeschwMessStartAlt = windGeschwMessStart; // merken uns Startzeit 
   windGeschwPinZaehler = 0; // setzen Pulszaehler auf 0
   windGeschwPinStatusAlt = HIGH; // setzen PulsStatus auf HIGH
  
   while ((windGeschwMessStart - windGeschwMessStartAlt) <= windGeschwMessZeit) // solange die 10000 ms (10 Sekunden) noch nicht durch sind ..
   {
      windGeschwPinStatus = digitalRead(windGeschwPin); // lese Eingang Pin 6 digital 
      if (windGeschwPinStatus != windGeschwPinStatusAlt) // wenn sich Pin-Status geändert hat ...
      {                
         if (windGeschwPinStatus == LOW) // und wenn  alter Status = HIGH
         {                   
           windGeschwPinZaehler++; // erhoehe Zaehler um 1
         }
      } 
   windGeschwPinStatusAlt = windGeschwPinStatus; // merken uns Pin-Status fuer naechsten Durchlauf
   windGeschwMessStart = millis(); // aktualisieren Zeit 
   }
   
   // windGeschw =  ((windGeschwPinZaehler * 24) / 10) / (windGeschwMessZeit / 1000); // ein Impuls = 2,4 km/h
   
   windGeschw =  windGeschwPinZaehler * 2.4; // ein Impuls = 2,4 km/h, aufgerundet
   windGeschw = (windGeschw / (windGeschwMessZeit / 1000)); // teilen durch Messzeit in Sekunden
   // ermitteln windstaerke nach Beaufort
   if (windGeschw >= 0 && windGeschw <= 2)
   {
     beaufort = 0;
   }
   else if (windGeschw > 2 && windGeschw <= 5)
   {
     beaufort = 1;
   }
   else if (windGeschw > 5 && windGeschw <= 11)
   {
     beaufort = 2;
   }
   else if (windGeschw > 11 && windGeschw <= 19)
   {
     beaufort = 3;
   }
   else if (windGeschw > 19 && windGeschw <= 28)
   {
     beaufort = 4;
   }
   else if (windGeschw > 28 && windGeschw <= 38)
   {
     beaufort = 5;
   }
   else if (windGeschw > 38 && windGeschw <= 49)
   {
     beaufort = 6;
   }
   else if (windGeschw > 49 && windGeschw <= 61)
   {
     beaufort = 7;
   }
   else if (windGeschw > 61 && windGeschw <= 74)
   {
     beaufort = 8;
   }
   else if (windGeschw > 74 && windGeschw <= 88)
   {
     beaufort = 9;
   }
   else if (windGeschw > 89 && windGeschw <= 102)
   {
     beaufort = 10;
   }
   else if (windGeschw > 102 && windGeschw <= 117)
   {
     beaufort = 11;
   }
   else
   {
     beaufort = 12;
   }
    
   // Ausgabe der Messwerte auf seriellen Konsole 
   //Serial.print(windGeschwPinZaehler);
   //Serial.print(" Windgeschwindigkeit: ");
   //Serial.print(windGeschw);
   //Serial.print(" km/h");
   //Serial.print(" Windstaerke: ");
   //Serial.print(beaufort);
   //Serial.println(" Beaufort"); 
   {;} // verlassen Subroutine, nicht vergessen!
}

void printMeasurement() {
  float tem = sht1x.readTemperatureC();
  float hum = sht1x.readHumidity();
  
  Serial.print("SPD: ");
  Serial.print(beaufort);
  Serial.print(";");
  Serial.print("DIR: ");
  Serial.print(windText1);
  Serial.print(windText2);
  Serial.print(windText3);
  Serial.print(windText4);
  Serial.print(";");
  Serial.print("RIN: ");
  Serial.print(rainCounter * 0.2794);
  Serial.print(";");
  Serial.print("TEM: ");
  Serial.print(tem);
  Serial.print(";");
  Serial.print("HUM: ");
  Serial.print(hum);
  Serial.print(";");
  Serial.print("PRE: ");
  double T,P;
  char result = bmp.startMeasurment();
 
  if(result!=0){
    delay(result);
    result = bmp.getTemperatureAndPressure(T,P);
      if(result!=0)
      {
        Serial.print(P,2);
      }
      else {
        Serial.println("-1");
      }
  }
  else {
    Serial.println("-1");
  }
  Serial.print(";");
  Serial.print("LUX: ");
  Serial.print(luxsensor.readLux());
  Serial.print(";");
  Serial.print("UV: ");
  Serial.print(readUV());
  Serial.println(";");

  rainCounter = 0;
}

boolean debounceRain(boolean last)
{
  boolean current = digitalRead(rainPin);
  if (last != current)
  {
    delay(5);
    current = digitalRead(rainPin);
  }
  return current;
}

void readRain()
{
  //currentButtonRain = debounceRain(lastButtonRain);
  //if (lastButtonRain == LOW && currentButtonRain == HIGH)
  //{
  rainCounter++;
  //}
  //lastButtonRain = currentButtonRain;
  
}

float readUV() {

  byte msb=0, lsb=0;
  uint16_t uv;

  Wire.requestFrom(I2C_ADDR_UV+1, 1); // MSB (erstes byte am sensor lesen)
  delay(1);
  if(Wire.available())
    msb = Wire.read();

  Wire.requestFrom(I2C_ADDR_UV+0, 1); // LSB (zweites byte am sensor lesen)
  delay(1);
  if(Wire.available())
    lsb = Wire.read();

  uv = (msb<<8) | lsb;   // bytes durch Bitshift zu einer Zahl verbinden

  //Serial.print("uW je cm²: ");
  //Serial.println(uv, DEC);            // Ausgabe als 16bit integer
  //Serial.print("UV-Index: ");
  //Serial.println(getUVI(uv));
  return getUVI(uv);
}

/*
 * getUVI()
 * erwartet den Messert des UV-Sensors als Eingabeparameter
 * und gibt den entsprechenden Wert auf dem UV-Index zurück
 */
float getUVI(int uv){
  float uvi = refVal*(uv*5.625)/1000;
  return uvi;
}


