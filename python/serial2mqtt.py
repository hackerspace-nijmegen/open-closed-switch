#!/usr/bin/env python3
import serial
import paho.mqtt.client as mqtt

mqttclient =mqtt.Client("BarSwitch")
mqttclient.connect("127.0.0.1")

with serial.Serial('/dev/ttyUSB0') as ser:
    while True:
        line = ser.readline().decode('utf-8')
        # print("got line'%s'"%line);
        if line.startswith("CLOSED"):
            mqttclient.publish("hsnmgn/spacestatus","CLOSED")
        elif line.line.startswith("OPEN"):
            mqttclient.publish("hsnmgn/spacestatus","OPEN")
        else:
            print("unexpected input from arduino '%s'"%line)
