#include <Arduino.h>
#include "HardwareSerial.h"
#include "bg95.h"

Bg95 nbiot;

void setup() {
  // put your setup code here, to run once:
  DEBUG.begin(9600);
  BG95.begin(115200,SERIAL_8N1,16,17);
  // BG95.println("AT");
  // DEBUG.println(BG95.readString());
  // BG95.println("AT+QMTOPEN=1,\"broker.hivemq.com\",1883");
  // DEBUG.println(BG95.readString());
  // bool flag = nbiot.begin();
  bool flag = nbiot.connectMqtt(BROKER, "localhost","");
  DEBUG.println(flag);
  DEBUG.println(nbiot.debug);
}

void loop() {
  // put your main code here, to run repeatedly:
  DEBUG.println(BG95.readString());
  delay(1000);
}

