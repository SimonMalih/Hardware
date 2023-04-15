#pragma once

#define LIGHT_SENSOR_PIN 32

#include <Arduino.h>
#include "Device.h"

class LightSensor : public Device {
   public:
    LightSensor(std::string position) : Device(position, Device::LIGHTSENSOR) {}

    void read() {
        intValue = analogRead(LIGHT_SENSOR_PIN);
    }

    void printValue() {
        Serial.print("Analog Value = ");
        Serial.print(intValue);  // the raw analog reading
        if (intValue < 40) {
            Serial.println(" => Dark");
        } else if (intValue < 800) {
            Serial.println(" => Dim");
        } else if (intValue < 2000) {
            Serial.println(" => Light");
        } else if (intValue < 3200) {
            Serial.println(" => Bright");
        } else {
            Serial.println(" => Very bright");
        }
    }
};