#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include <DallasTemperature.h>
#include <OneWire.h>
#include "Device.h"

const int oneWireBus = 4;
OneWire oneWire(4);
DallasTemperature sensors(&oneWire);

class Temperature : public Device {
   public:
    Temperature(std::string position) : Device(position, THERMOMETER) {
        sensors.begin();
    }

    void getTemperature() {
        sensors.requestTemperatures();
        float temperatureC = sensors.getTempCByIndex(0);
        int temperature = (int)temperatureC;
        intValue = temperatureC;
        // float temperatureF = sensors.getTempFByIndex(0);
        Serial.print(temperature);
        Serial.println("ºC");
        // Serial.print(temperatureF);
        // Serial.println("ºF");
    }
};

#endif

// #ifndef TEMPERATURE_H_
// #define TEMPERATURE_H_

// #include <DallasTemperature.h>
// #include <OneWire.h>

// const int oneWireBus = 4;
// OneWire oneWire(4);
// DallasTemperature sensors(&oneWire);

// class Temperature : public Device {
//    public:
//     Temperature(std::string position) : Device(position, THERMOMETER) {
//         //sensors.begin();
//     }

// void start() {
//         sensors.begin();
// }

//     void getTemperature() {
//         float temperatureC = sensors.getTempCByIndex(0);
//         Serial.print(temperatureC);
//         Serial.println("ºC");
//     }
// };

// #endif