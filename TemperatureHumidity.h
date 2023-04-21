#pragma once
#include <Arduino.h>
#include <DHT.h>
#include <Device.h>

#define DHT_SENSOR_PIN 21 
#define DHT_SENSOR_TYPE DHT11

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

class TemperatureHumidity {
   public:
    int temperature;
    int humidity;
    
    void start() {
        dht_sensor.begin();
    }

    void readSensor() {
        int temp = (int)dht_sensor.readTemperature();
        int hum = (int)dht_sensor.readHumidity();

        temperature = temp < 300 ? temp : 0;
        humidity = hum < 300 ? hum : 0;
    }

    void printValues() {
        printf("Temperature: %d C\n", temperature);
        printf("Humidity: %d\n", humidity);
    }
};