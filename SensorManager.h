#pragma once

#include <Arduino.h>
#include "Database.h"
#include "LightBulb.h"
#include "LightSensor.h"
#include "TemperatureHumidity.h"

#define LIGHT1_PIN 16
#define LIGHT2_PIN 17
#define LIGHT3_PIN 18

class SensorManager {
   private:
    Database database;
    Lightbulb light1 = Lightbulb("1");
    Lightbulb light2 = Lightbulb("2");
    Lightbulb light3 = Lightbulb("3");
    LightSensor lightSensor = LightSensor("1");
    TemperatureHumidity thSensor = TemperatureHumidity();
    int delay = 30000;
    unsigned long previousTime = 0;

   public:
    void start() {
        database = Database();
        dht_sensor.begin();
    }

    void readSensors() {
        database.read(light1);
        digitalWrite(LIGHT1_PIN, light1.getBool());
        database.read(light2);
        digitalWrite(LIGHT2_PIN, light2.getBool());
        database.read(light3);
        digitalWrite(LIGHT3_PIN, light3.getBool());

        if (millis() - previousTime > delay) {
            previousTime = millis();
            printf("reading sensors\n");
            lightSensor.read();
            database.write(lightSensor);
            thSensor.readSensor();
            database.writeTemperature(thSensor.temperature);
            database.writeHumidity(thSensor.humidity);
        }

    }

    void printSensors() {
        printf("Light1 : %d\n", light1.intValue);
        printf("Light2 : %d\n", light2.intValue);
        printf("Light3 : %d\n", light3.intValue);
        thSensor.printValues();
        lightSensor.printValue();
    }
};