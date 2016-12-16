/*
BME280 Test.ino
This code shows how to record data from the BME280 environmental sensor.
This file is an example file, part of the Arduino BME280 library.
Copyright (C) 2016  Tyler Glenn

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Written: Dec 30 2015.
Last Updated: Jan 1 2016.

Connecting the BME280 Sensor:
Sensor              ->  Board
-----------------------------
Vin (Voltage In)    ->  3.3V
Gnd (Ground)        ->  Gnd
SDA (Serial Data)   ->  A4 on Uno/Pro-Mini, 20 on Mega2560/Due, 2 Leonardo/Pro-Micro
SCK (Serial Clock)  ->  A5 on Uno/Pro-Mini, 21 on Mega2560/Due, 3 Leonardo/Pro-Micro

 */

/* ==== Includes ==== */
#include <BME280.h>
/* ====  END Includes ==== */

/* ==== Defines ==== */
#define SERIAL_BAUD 115200
/* ==== END Defines ==== */

/* ==== Global Variables ==== */
BME280 bme;                   // Default : forced mode, standby time = 1000 ms
                              // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,
bool metric = false;
/* ==== END Global Variables ==== */


/* ==== Prototypes ==== */
/* === Print a message to stream with the temp, humidity and pressure. === */
void printBME280Data(Stream * client);
/* === Print a message to stream with the altitude, and dew point. === */
void printBME280CalculatedData(Stream* client);
/* ==== END Prototypes ==== */

/* ==== Setup ==== */
void setup() {
  Serial.begin(SERIAL_BAUD);
  while(!Serial) {} // Wait
  while(!bme.begin()){
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }
}
/* ==== END Setup ==== */

/* ==== Loop ==== */
void loop() {
   printBME280Data(&Serial);
   printBME280CalculatedData(&Serial);
   delay(500);
}
/* ==== End Loop ==== */

/* ==== Functions ==== */
void printBME280Data(Stream* client){
  float temp(NAN), hum(NAN), pres(NAN);
   uint8_t pressureUnit(3);   // unit: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi
  bme.ReadData(pres, temp, hum, metric, pressureUnit);                // Parameters: (float& pressure, float& temp, float& humidity, bool hPa = true, bool celsius = false)
  /* Alternatives to ReadData():
    float ReadTemperature(bool celsius = false);
    float ReadPressure(uint8_t unit = 0);
    float ReadHumidity();

    Keep in mind the temperature is used for humidity and
    pressure calculations. So it is more effcient to read
    temperature, humidity and pressure all together.
   */
  client->print("Temp: ");
  client->print(temp);
  client->print("°"+ String(metric ? 'C' :'F'));
  client->print("\t\tHumidity: ");
  client->print(hum);
  client->print("% RH");
  client->print("\t\tPressure: ");
  client->print(pres);
  client->print(" atm");
}
void printBME280CalculatedData(Stream* client){
  float altitude = bme.CalculateAltitude(metric);
  float dewPoint = bme.CalculateDewPoint(metric);
  client->print("\t\tAltitude: ");
  client->print(altitude);
  client->print((metric ? "m" : "ft"));
  client->print("\t\tDew point: ");
  client->print(dewPoint);
  client->println("°"+ String(metric ? 'C' :'F'));

}
/* ==== END Functions ==== */
