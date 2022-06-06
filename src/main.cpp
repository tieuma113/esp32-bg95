#include <Arduino.h>
#include "HardwareSerial.h"
#include "bg95.h"
#include <time.h>

#define BROKER              "42.115.161.106"
Bg95 nbiot;

void setup() {
  // put your setup code here, to run once:
  // DEBUG.begin(9600);
  // BG95.begin(115200,SERIAL_8N1,16,17);
  // BG95.println("AT");
  // DEBUG.println(BG95.readString());
  // BG95.println("AT+QMTOPEN=1,\"broker.hivemq.com\",1883");
  // DEBUG.println(BG95.readString());
  bool flag = nbiot.begin();
  DEBUG.println(flag);
  flag = nbiot.connectMqtt(BROKER, "localhost","");
  DEBUG.println(flag);
  DEBUG.println(nbiot.debug);
  flag = nbiot.subscribeMqtt("NbIoT/Farm/+",2);
  DEBUG.println(flag);
}

void loop() {
  // put your main code here, to run repeatedly:
  long l_ran1 = rand() % 99 + 10;
  long l_ran2 = rand() % 99 + 10;
  bool flag = false;
  DEBUG.println(BG95.readString());
  String data = String(l_ran1);
  flag = nbiot.sendMqtt("NbIoT/Farm/Temp", data, 2);
  DEBUG.println(flag);
  data = String(l_ran2);
  flag = nbiot.sendMqtt("NbIoT/Farm/Humi", data, 2);
  delay(1000);
}

