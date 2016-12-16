/*
* Copyright (c) 2015 SODAQ. All rights reserved.
*
* This file is part of Sodaq_RN2483.
*
* Sodaq_RN2483 is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as
* published by the Free Software Foundation, either version 3 of
* the License, or(at your option) any later version.
*
* Sodaq_RN2483 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with Sodaq_RN2483.  If not, see
* <http://www.gnu.org/licenses/>.
*/

#include <Sodaq_RN2483.h>

// MBili
#define debugSerial Serial
// Autonomo
//#define debugSerial SerialUSB
#define loraSerial Serial1

const uint8_t devAddr[4] =
{
	0x00, 0x1A, 0x62, 0xAE
};

// USE YOUR OWN KEYS!
const uint8_t appSKey[16] =
{
	0x0D, 0x0E, 0x0A, 0x0D,
	0x0B, 0x0E, 0x0E, 0x0F,
	0x0C, 0x0A, 0x0F, 0x0E,
	0x0B, 0x0A, 0x0B, 0x0E,
};

// USE YOUR OWN KEYS!
const uint8_t nwkSKey[16] =
{
	0x0D, 0x0E, 0x0A, 0x0D,
	0x0B, 0x0E, 0x0E, 0x0F,
	0x0C, 0x0A, 0x0F, 0x0E,
	0x0B, 0x0A, 0x0B, 0x0E,
};

void setup()
{
	debugSerial.begin(57600);
	loraSerial.begin(LoRaBee.getDefaultBaudRate());

	if (LoRaBee.initABP(loraSerial, devAddr, appSKey, nwkSKey, true))
	{
		debugSerial.println("Connection to the network was successful.");
	}
	else
	{
		debugSerial.println("Connection to the network failed!");
	}
}

void loop()
{
  while (debugSerial.available()) 
  {
    loraSerial.write((char)debugSerial.read());
  }

  while (loraSerial.available()) 
  {
    debugSerial.write((char)loraSerial.read());
  }
}
