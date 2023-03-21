#pragma once

#include <Arduino.h>
#include <HTTPClient.h>
#include <Keypad.h>
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include "GlobalSettings.h"
#include "LCDDisplay.h"
#include "Network.h"
#include "RFID.h"

#define ROW_NUM 4     // four rows
#define COLUMN_NUM 4  // four columns
#define MENU 0
#define PIN 1
#define FINGERPRINT 2
#define ID 3

class AuthManager {
   private:
    String buffer = "";
    char keys[ROW_NUM][COLUMN_NUM] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}};

    byte pin_rows[ROW_NUM] = {19, 18, 5, 17};     // GIOP19, GIOP18, GIOP5, GIOP17 connect to the row pins
    byte pin_column[COLUMN_NUM] = {16, 4, 0, 2};  // GIOP16, GIOP4, GIOP0, GIOP2 connect to the column pins
    Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);
    RFID rfid = RFID();

   public:
    LCDDisplay lcdManager = LCDDisplay();
    int mode = MENU;
    bool auth = false;
    String pin;
    int delayTime = 10000;
    int doorDelayTime = 10000;
    int rfidDelay = 10000;
    unsigned long previousTime = 0;
    unsigned long previousDoorTimer = 0;
    unsigned long previousRfid = 0;
    String uid = "Default";

    void getKey() {
        digitalWrite(32, auth);

        if (auth) {
            if (millis() - previousTime > delayTime) {
                auth = false;
                mode = 0;
                lcdManager.menu();
            }
        }

        if(mode == ID) {
            if (millis() - previousRfid > rfidDelay) {
                mode = 0;
                lcdManager.menu();
            } else {
                if(!auth)
                    rfid.read(auth, previousTime);
            }
        }
        char key = keypad.getKey();

        if (mode == 0) {
            menuMode(key);
            return;
        }

        if (key == 'C') {
            reset();
            return;
        }

        switch (mode) {
            case 1:
                pinMode(key);
                break;
            case 2:
                fingerprintMode(key);
                break;
            case 3:
                rfidMode();
                break;
            default:
                break;
        }
    }

    void start() {
        lcdManager.start();
        reset();
        rfid.start();
    }

    void setPin(string s) {
        pin = "";
        for (char c : s) {
            pin += c;
        }
    }

    void menuMode(char key) {
        if (key) {
            switch (key) {
                case 'A':
                    mode = 1;
                    buffer = "";
                    lcdManager.pinMode("");
                    break;
                case 'B':
                    mode = 2;
                    fingerprintMode(key);
                    break;
                case 'C':
                    mode = 3;
                    lcdManager.rfidMode();
                    rfidMode();
                    break;
                case 'D':
                    lcdManager.background();
                    break;
                default:
                    break;
            }
        }
    }

    void pinMode(char c) {
        if (c == 'D') {
            bool success = authenticate(buffer);
            lcdManager.authentication(success);
            if (success) {
                auth = true;
                previousTime = millis();
                reset();
                lcdManager.menu();
            } else {
                mode = 1;
                buffer = "";
                lcdManager.pinMode("");
            }
            return;
        } else if (c > 48 && c < 58) {
            printf("%c\n", &c);
            buffer += String(c);
            lcdManager.pinMode(buffer);
        }
    }

    bool authenticate(String input) {
        String basePin = "9987";
        return (basePin.compareTo(buffer)) == 0 || (pin.compareTo(buffer) == 0);
    }

    void fingerprintMode(char c) {
        if (mode != 2) {
            lcdManager.fingerprintMode();
        }

        if (c == 'C') {
        }
    }

    void rfidMode() {
        previousRfid = millis();
    }

    void reset() {
        mode = 0;
        lcdManager.menu();
    }

    void enterCode() {
        Serial.println("Enter pressed");
        if (buffer.equals("1234"))
            Serial.println("Correct password");
        else
            Serial.println("Incorrect password");
        buffer = "";
    }

    void openLink(String url) {
        if ((WiFi.status() == WL_CONNECTED)) {  // Check the current connection status

            // HTTPClient http;

            // http.begin(url);            // Specify the URL
            // http.GET();

            // int httpCode = http.GET();  // Make the request

            // if (httpCode > 0) {  // Check for the returning code
            //     // String payload = http.getString();
            //     // Serial.println(httpCode);
            //     // Serial.println(payload);
            // } else {
            //     Serial.println("Error on HTTP request");
            // }
            //     http.end();  // Free the resources
            // } else {
            //    printf("HTTP REQUEST FAILED, NOT CONNECTED TO THE INTERNET\n");
        }
    }

    void sendNotification(bool success) {
        // srand(time(0));
        String random = to_string(rand()).c_str();

        String url = "https://us-central1-iothome-a8984.cloudfunctions.net/writeNotification?uid=" + uid;
        if (success) {
            url += "&type=entry";
        } else {
            url += "&type=intruder";
        }
        url += +"&random=" + random;
        printf("link is: %s\n", url.c_str());
        openLink(url.c_str());
    }
};

