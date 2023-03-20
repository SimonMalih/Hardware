#ifndef LCDDISPLAY_H_
#define LCDDISPLAY_H_

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "Arduino.h"

int lcdColumns = 16;
int lcdRows = 2;

// lcd1 - 0x27(39) | lcd2 - 0x25(3)

LiquidCrystal_I2C lcd1(0x27, lcdColumns, lcdRows);
LiquidCrystal_I2C lcd2(0x25, lcdColumns, lcdRows);

class LCDDisplay {
   private:
    bool backlightOn = true;

   public:
    void start() {
        lcd1.init();
        lcd1.backlight();
        lcd2.init();
        lcd2.backlight();
    }

    void findDisplayAddresses() {
        Wire.begin();
        Serial.println("\nI2C Scanner");

        byte error, address;
        int nDevices;
        Serial.println("Scanning...");
        nDevices = 0;
        for (address = 1; address < 127; address++) {
            Wire.beginTransmission(address);
            error = Wire.endTransmission();
            if (error == 0) {
                Serial.print("I2C device found at address 0x");
                if (address < 16) {
                    Serial.print("0");
                }
                Serial.println(address, HEX);
                nDevices++;
            } else if (error == 4) {
                Serial.print("Unknow error at address 0x");
                if (address < 16) {
                    Serial.print("0");
                }
                Serial.println(address, HEX);
            }
        }
        if (nDevices == 0) {
            Serial.println("No I2C devices found\n");
        } else {
            Serial.println("done\n");
        }
    }

    void background() {
        if (backlightOn) {
            lcd1.noBacklight();
            lcd2.noBacklight();
        } else {
            lcd1.backlight();
            lcd2.backlight();
        }
        backlightOn = !backlightOn;
    }

    void menu() {
        String lcd1Menu[2] = {"[A] Pin", "[B] Finger Print"};
        String lcd2Menu[2] = {"[C] RFID", "[D] Backlight"};
        print(lcd1Menu, 1);
        print(lcd2Menu, 2);
    }

    String displayCode(String s) {
        String codeString = "____";

        for (int i = 0; i < s.length(); i++) {
            codeString[i] = '*';
        }

        return codeString;
    }

    void pinMode(String password) {
        String lcd1PinMode[2] = {"Enter your pin!", displayCode(password)};
        String lcd2PinMode[2] = {"[C] to cancel", "[D] to enter"};
        print(lcd1PinMode, 1);
        print(lcd2PinMode, 2);
    }

    void fingerprintMode() {
        String lcd1FingerprintMode[2] = {"Place finger ", "on scanner"};
        String lcd2FingerprintMode[2] = {"[C] to cancel", ""};
        print(lcd1FingerprintMode, 1);
        print(lcd2FingerprintMode, 2);
    }

    void rfidMode() {
        String lcd1PinMode[2] = {"Tap RFID card", "on the scanner"};
        String lcd2PinMode[2] = {"[C] to cancel", ""};
        print(lcd1PinMode, 1);
        print(lcd2PinMode, 2);
    }

    void authentication(bool success) {
        String message = success ? "SUCCESS" : "INVALID";
        String s[2] = {"Enter your pin!", message};
        print(s, 0);
        delay(1000);
    }

    void clear() {
        lcd1.clear();
        lcd2.clear();
    }

    void print(String s, int display, int line, int position) {
        LiquidCrystal_I2C* currentLCD = display == 2 ? &lcd2 : &lcd1;
        currentLCD->clear();
        currentLCD->setCursor(position, line);
        currentLCD->print(s);
    }

    void print(String s, int display, int line) {
        if (display == 2) {
            lcd2.clear();
            lcd2.setCursor(0, line);
            lcd2.print(s);
        } else {
            lcd1.clear();
            lcd1.setCursor(0, line);
            lcd1.print(s);
        }
    }

    void print(String s[2], int display) {
        LiquidCrystal_I2C* currentLCD = display == 2 ? &lcd2 : &lcd1;
        currentLCD->clear();
        currentLCD->setCursor(0, 0);
        currentLCD->print(s[0]);
        currentLCD->setCursor(0, 1);
        currentLCD->print(s[1]);
    }
};

#endif