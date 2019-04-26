#include "SenseBoxMCU.h"

const long interval = 60000;
long time_start = 0;
long time_actual = 0;
float p10,p25;


SDS011 my_sds(Serial1);

Bee* b = new Bee();

HDC1080 hdc;

TSL45315 tsl;

VEML6070 veml;

BMP280 bmp_sensor;

OpenSenseMap osem("5caee5a0c9e9ab001a306765",b);


void setup() {
  b->connectToWifi("SSID","Password");
delay(1000);
  hdc.begin();
  tsl.begin();
  veml.begin();
  bmp_sensor.begin();
  Serial1.begin(9600);

}

void loop() {
  time_start = millis();
  if (time_start > time_actual + interval) {
    time_actual = millis();
       osem.uploadMeasurement(hdc.getTemperature(),"5caee5a0c9e9ab001a30676a");
       osem.uploadMeasurement(tsl.getIlluminance(),"SensorID");
       osem.uploadMeasurement(veml.getUvIntensity(),"SensorID");
       osem.uploadMeasurement(bmp_sensor.getPressure(),"SensorID");
       osem.uploadMeasurement(my_sds.getPm10(),"SensorID");
       osem.uploadMeasurement(my_sds.getPm25(),"SensorID");
  }

}