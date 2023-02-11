#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#define ROW_NUM 4     // four rows
#define COLUMN_NUM 4  // four columns

#include <Arduino.h>
#include <Keypad.h>
#include <string>
#include "LCDDisplay.h"

// keypad from pins 2 to 18


// 0 = menu
// 1 = pin
// 2 = fingerprint
// 3 = rfid

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

   public:
    LCDDisplay lcdManager = LCDDisplay();
    int mode = 0;
    void getKey() {
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

    void start() {
        lcdManager.start();
        reset();
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
                    fingerprintMode();
                    break;
                case 'C':
                    mode = 3;
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
            delay(3000);
            if (success) {
                reset();
            } else {
                mode = 1;
                buffer = "";
                lcdManager.pinMode("");
            }
            return;
        } else if (c > 48 && c < 58) {
            buffer += String(c);
            lcdManager.pinMode(buffer);
        }
    }

    bool authenticate(String input) {
        // get password from firebase database or maybe needs to be set
        // manual or set when account is created and can only be viewed from
        // the app
        String pin = "9987";
        return (pin.compareTo(input)) == 0;
    }

    void fingerprintMode() {
        lcdManager.fingerprintMode();
    }

    void rfidMode() {
        lcdManager.rfidMode();
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
};

#endif
