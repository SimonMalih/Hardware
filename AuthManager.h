#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#define ROW_NUM 4     // four rows
#define COLUMN_NUM 4  // four columns

#include <Arduino.h>
#include <Keypad.h>
#include <string>
#include "LCDDisplay.h"
#include "RFID.h"

// keypad from pins 2 to 18
// 0 = menu
// 1 = pin
// 2 = fingerprint
// 3 = rfid

//PWMLED led = PWMLED("", 32, 0);

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
    int mode = 0;
    bool auth;
    String pin;
    int delayTime = 10000;
    int doorDelayTime = 10000;
    int rfidDelay = 10000;
    unsigned long previousTime = 0;
    unsigned long previousDoorTimer = 0;
    unsigned long previousRfid = 0;

    // void getRfid() {
    //     RFID rfid = RFID();

    //     rfid.start();

    //     // printf("%lf", (millis() - previousRfid > rfidDelay));
    //     while (!auth && (millis() - previousRfid < rfidDelay)) {
    //         printf("in loop b\n");
    //         rfid.read(auth);
    //     }
    //     mode = 0;
    //     lcdManager.menu();
    //     // printf("mode is now 0\n");
    // }

    void getKey() {
        digitalWrite(32, auth);
        char key = keypad.getKey();
        if (auth) {
            if (millis() - previousTime > delayTime) {
                auth = false;
            } else {
                return;
            }
        }

        if (mode == 3) {
            //getRfid();
            //printf("%lf", (millis() - previousRfid > rfidDelay));
            while (!auth && (millis() - previousRfid < rfidDelay)) {
                printf("in loop b\n");
                rfid.read(auth);
            }
            mode = 0;
            lcdManager.menu();
            //printf("mode is now 0\n");
        }



        // if(mode == 3) {

        //     // if (millis() - previousRfid > rfidDelay) {
        //     //     mode = 0;
        //     // } else {
        //     //     rfid.read(auth);
        //     //     return;
        //     // }
        //     // return;
        // } else {
        //     key = keypad.getKey();
        // }
        

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
                    previousRfid = millis();
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
        if(mode != 2) {
            lcdManager.fingerprintMode();
        }

        if (c == 'C') {
            
        } 
    }

    void rfidMode() {
        lcdManager.rfidMode();
        previousRfid = millis();
        // rfid.read(auth, previousTime);
        //  auth = true;
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

    // void doorHasOpened() {
    //     if (millis() - previousDoorTimer > doorDelayTime) {
    //         if(!auth) {
    //             // send message to user database that
    //             // send email and sms
    //         }
    //     }
    // }
};

#endif
