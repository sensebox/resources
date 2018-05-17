# senseBox resources
This repo contains additional or customized Arduino libraries and datasheets for senseBox sensors, as well as common assets such as logos and documentational images.

## Libraries
|Sensor|Homepage|Arduino Lib.|
|------|--------|------------|
|BMP280|[Bosch](https://www.bosch-sensortec.com/bst/products/all_products/bmp280)|[BMP280 by mahfuz195](https://github.com/mahfuz195/BMP280-Arduino-Library)|
|HDC1008|[Texas Instruments](http://www.ti.com/product/HDC1008)|[HDC100X by RFgermany](https://github.com/RFgermany/HDC100X_Arduino_Library)|
|TSL45315|[Austria Micro Systems](http://www.ams.com/eng/Products/Sensor-Driven-Lighting/SDL-Ambient-Light-Sensors/TSL45315)|[Makerblog_TSL45315 by Adi Dax](https://github.com/adidax/Makerblog_TSL45315)|
|VEML6070|[VISHAY](http://www.vishay.com/ppg?84277)|based on example from [watterott](https://github.com/watterott/VEML6070-Breakout/blob/master/software/VEML6070.ino)|
|SDS011|[inovafit](http://aqicn.org/sensor/sds011/)|modified version of [SDS011 by ricki-z](https://github.com/ricki-z/SDS011) that allows selection of the serial pins|

### senseBox MCU library
This repository also includes the required libraries for the new `senseBox MCU`. It is included as a submodule and can be found in the in the original repository [here](https://github.com/sensebox/SenseBoxMCU-Lib).

If you clone this repository initalize the submodule by executing:
```
git submodule update --init --recursive
```

You will find the submodule under `libraries/libraries/SenseBox-MCU`.

## Changelog
See [`Changelog.md`](CHANGELOG.md) for detailed information.

## Contribution
Please have a look at [`Contributing`](CONTRIBUTING.md) for detailed information.

#### further libraries
- [WiFi101](https://github.com/arduino-libraries/WiFi101/releases/tag/0.14.3)
- modified Arduino Ethernet lib
- [RV8523 realtime clock](https://github.com/watterott/Arduino-Libs/tree/master/RV8523)
- [Ethernet2](https://github.com/adafruit/Ethernet2)