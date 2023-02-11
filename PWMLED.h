#ifndef PWMLED_H_
#define PWMLED_H_

#include "Arduino.h"
#include "Device.h"

class PWMLED : public Device {
   private:
    const int freq = 5000;
    const int ledChannel = 0;
    const int resolution = 8;
    int ledPin;

    void initPWMLED( int pin, int brightness) {
        ledPin = pin;
        ledcAttachPin(ledPin, ledChannel);
        setBrightness(brightness);
    }

   public:
    PWMLED(std::string position, int pin, int brightness) : Device(position, Device::LIGHTBULB) {
        ledcSetup(ledChannel, freq, resolution);
        initPWMLED(pin, brightness);
    }

    PWMLED(std::string position, int pin) : Device(position, Device::LIGHTBULB) {
        ledcSetup(ledChannel, freq, resolution);
        initPWMLED(pin, 0);
    }

    void setValue(const int &n) override {
        intValue = n;
        setBrightness();
    }

    void setBrightness(int n) {
        if (n < 0)
            n = 0;
        if (n > 100)
            n = 100;
        int dutyCycle = n * 3;
        ledcWrite(ledChannel, dutyCycle);
    }

    void setBrightness() {
        Serial.printf("led is: %d\n", intValue);
        int dutyCycle = intValue * 2.55;
        ledcWrite(ledChannel, dutyCycle);
    }
};

#endif