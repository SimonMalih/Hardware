#ifndef Network_H_
#define Network_H_

#include <WiFi.h>

class Network {
   private:
    const char *WIFI_SSID = "Simon";
    const char *WIFI_PASSWORD = "Simon9987";

   public:
    Network() {
        initWiFi();
    }

    void initWiFi() {
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        delay(100);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        Serial.print("Connecting to Wi-Fi");

        while (WiFi.status() != WL_CONNECTED) {
            Serial.print(".");
            delay(300);
        }

        Serial.println();
        Serial.print("Connected with IP: ");
        Serial.println(WiFi.localIP());
        Serial.println();
    }
};

#endif