#ifndef __BG95_H__
#define __BG95_H__

#include <Arduino.h>
#include "HardwareSerial.h"
#include <string.h>
#include <stdlib.h>

#define DEBUG               Serial
#define BG95                Serial2
#define BROKER              "broker.hivemq.com"
#define PORT        		1883

class Bg95 {
    public:
        uint8_t debug;
        Bg95();
        bool begin();
        String getIP();
        String getIMEI();
        String getMqttBroker();
        String getClientName();
        String getClientPassword();
        void setClientName(const String name);
        void setClientPassword(const String password);
        void setMqttBroker(const String broker);
        bool shutdown();
        bool connectMqtt(String broker, String name, String password);
        bool subscribeMqtt(String topic, uint8_t qos);
        bool sendMqtt(String topic, String &data, int qos);

    private:
        String ip;
        String imei;
        uint16_t port;
        String mqttbroker;
        String clientName;
        String clientPassword;

        bool setup();
        bool isAlive();
        bool confNbiotConnection();
        bool checkOK(int timeout);
        bool networkRegistered();
        bool checkRespForOpSelection ();
        bool checkRespForReg ();

        bool connectBroker();
        bool checkConnection();
        bool connectClient();
        bool checkClientConnect();
        bool checkSubscription();
        bool checkSendMqtt();

        bool askForIp();
        bool askForImei();
        String checkImei();
        String checkIp();

        void sendAT(String atCommand);
        String receiveAT();
        void printAT(String text);
        bool checkRespForPoweredDown ();

};


#endif