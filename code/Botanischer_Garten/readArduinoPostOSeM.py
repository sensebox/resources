#!/usr/bin/python

import serial, string, time
import requests
import json

# input your senseBoxID
senseBoxID = "YOUR SENSEBOX ID"

output = " "

# specifing the USB port, baud rate at 9600
ser = serial.Serial('/dev/ttyUSB0', 9600, 8, 'N', 1, timeout=1)

def postMeasurement(sensorID, value):
    params = {"value": value }
    headers = {"Content-type": "application/json"}
	url = "https://api.opensensemap.org/boxes/%s/%s" % (senseBoxID, sensorID)

	r = requests.post(url, json=params, headers=headers)
	print r.content

# read output of arduino
# output of arduino has to be in following format:
# e.g.: SPD: 12.5; DIR; 50.3; ...
while 1 :
    out = ser.readline()
    if out != "":
	measurements = out.split(";")
	postMeasurement("YOUR WIND SPEED SENSOR ID", measurements[0].split(": ")[1]) # in example above: SPD
	postMeasurement("YOUR WIND DIRECTION SENSOR ID", measurements[1].split(": ")[1]) # in example above: DIR
	postMeasurement("YOUR RAIN SENSOR ID", measurements[2].split(": ")[1]) # ...
	postMeasurement("YOUR TEMPERATURE SENSOR ID", measurements[3].split(": ")[1])
	postMeasurement("YOUR HUMIDITY SENSOR ID", measurements[4].split(": ")[1])
	postMeasurement("YOUR BAROMETRIC PRESSURE SENSOR ID", measurements[5].split(": ")[1])
	postMeasurement("YOUR LIGHT SENSOR ID", measurements[6].split(": ")[1])
	postMeasurement("YOUR UV LIGHT SENSOR ID", measurements[7].split(": ")[1])
