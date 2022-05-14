#ifndef __BG95_H__
#define __BG95_H__

#include <Arduino.h>
#include "HardwareSerial.h"
#include <string.h>

#define DEBUG               Serial
#define BG95                Serial2

class Bg95 {
    public:
        Bg95();
        bool begin();
        String getIP();
        String getIMEI();
        bool openSocket();
        bool closeSocket();
        bool sendData();
        bool receiveData();
        bool shutdown();

    private:
        String ip;
        String imei;
        uint8_t socket;

        bool setup();
        bool isAlive();
        bool confNbiotConnection();
        bool checkOK(int timeout);
        bool networkRegistered();
        bool checkRespForOpSelection ();
        bool checkRespForReg ();

        bool askForIp();
        bool askForImei();
        String checkImei();
        String checkIp();

        void sendAT(String atCommand);
        String receiveAT();
        void printAT(String text);

};


#endif