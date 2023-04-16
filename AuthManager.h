#pragma once

#include <Arduino.h>
#include <Keypad.h>
#include <time.h>

#include <cstdlib>
#include <iostream>
#include <string>

#include "FingerprintScanner.h"
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
#define SOLENOID_LOCK_PIN 32
#define DOOR_SENSOR_PIN 26

class AuthManager {
   private:
    String buffer = "";
    char keys[ROW_NUM][COLUMN_NUM] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}};

    byte pin_rows[ROW_NUM] = {19, 18, 5, 33};     // GIOP19, GIOP18, GIOP5, GIOP33 connect to the row pins
    byte pin_column[COLUMN_NUM] = {25, 4, 0, 2};  // GIOP25, GIOP4, GIOP0, GIOP2 connect to the column pins
    Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);
    RFID rfid = RFID();
    FingerprintScanner fingerScanner = FingerprintScanner();
    int mode = MENU;
    bool auth = false;
    int delayTime = 10000;
    int doorDelayTime = 10000;
    int rfidDelay = 10000;
    int fingerDelay = 10000;
    int messageDelay = 10000;
    unsigned long previousTime = 0;
    unsigned long previousDoorTimer = 0;
    unsigned long previousRfid = 0;
    unsigned long previousFinger = 0;
    unsigned long previousMessage = 0;
    int doorDelay = 10000;
    String pin = "";
    int updateDelay = 45000;
    int doorState = 0;
    unsigned long previousUpdateTime = 0;
    GlobalSettings globalSettings;
    Database database;
    int doorSensorPin = 26;

    void checkDoor() {
        doorState = digitalRead(doorSensorPin);
        if (doorState == HIGH && !auth) {
            if (millis() - previousDoorTimer > doorDelay) {
                printf("Intruder message sent!\n");
                globalSettings.sendMessage(false);
                previousDoorTimer = millis();
            }
        }
    }

    void grantAccess() {
        lcdManager.accessGranted();
        digitalWrite(SOLENOID_LOCK_PIN, auth);
        globalSettings.sendMessage(true);
    }

   public:
    LCDDisplay lcdManager = LCDDisplay();

    void start() {
        lcdManager.start();
        lcdManager.wifi();
        Network();
        globalSettings = GlobalSettings();
        lcdManager.firebase();
        database = Database();
        lcdManager.readingUser();
        database.readUserInfo(globalSettings);
        previousUpdateTime = millis();
        lcdManager.sensorSetUp();
        fingerScanner.fsSetup();
        rfid.start();
        reset();
    }

    void getKey() {
        digitalWrite(SOLENOID_LOCK_PIN, auth);

        // checkDoor();

        if (millis() - previousUpdateTime < updateDelay) {
            // printf("Update not ready\n");
        } else {
            previousUpdateTime = millis();
            database.readUserInfo(globalSettings);
        }

        if (auth) {
            if (millis() - previousTime > delayTime) {
                auth = false;
                mode = 0;
                lcdManager.menu();
            }
            return;
        }

        if (mode == ID) {
            if (millis() - previousRfid > rfidDelay) {
                mode = 0;
                lcdManager.menu();
            } else {
                if (!auth) {
                    rfid.read(auth, previousTime);
                    if (auth) {
                        grantAccess();
                    }
                }
            }
        }

        if (mode == FINGERPRINT) {
            if (millis() - previousFinger > fingerDelay) {
                mode = 0;
                lcdManager.menu();
            } else {
                if (!auth) {
                    fingerScanner.readSensor(auth, previousFinger);
                    if (auth) {
                        grantAccess();
                    }
                }
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
                fingerprintMode();
                break;
            case 3:
                rfidMode();
                break;
            default:
                break;
        }
    }

    void setPin(string s) {
        pin = "";
        for (char c : s) {
            pin += c;
        }
    }

    void reconnectingWifi() {
        lcdManager.wifi();
        Network();
    }

    void wifiStatus() {
        bool connected = Network::isWifiConnected();
        lcdManager.wifiStatus(connected);
        if (!connected) {
            start();
        } else {
            lcdManager.menu();
        }
    }

    void updateUserSettings() {
        start();
        lcdManager.menu();
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
                    lcdManager.fingerprintMode();
                    fingerprintMode();
                    break;
                case 'C':
                    mode = 3;
                    lcdManager.rfidMode();
                    rfidMode();
                    break;
                case 'D':
                    lcdManager.background();
                    break;
                case '*':
                    wifiStatus();
                    break;
                case '#':
                    updateUserSettings();
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
                grantAccess();
            } else {
                mode = 1;
                buffer = "";
                lcdManager.pinMode("");
            }
            return;
        } else if (c >= '0' && c <= '9') {
            if (buffer.length() <= 4) {
                buffer += String(c);
                printf("%c\n", c);
                lcdManager.pinMode(buffer);
            }
        }
    }

    bool authenticate(String input) {
        String basePin = "9987";
        String currentPin = globalSettings.pin.c_str();
        return (basePin.compareTo(buffer)) == 0 || (currentPin.compareTo(buffer) == 0);
    }

    void fingerprintMode() {
        previousFinger = millis();
    }

    void rfidMode() {
        previousRfid = millis();
    }

    void reset() {
        mode = 0;
        lcdManager.menu();
    }
};