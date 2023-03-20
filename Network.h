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

    // static void openLink(String url) {
    //     if ((WiFi.status() == WL_CONNECTED)) {  // Check the current connection status

    //         HTTPClient http;

    //         http.begin(url);  // Specify the URL
    //         int httpCode = http.GET();                                         // Make the request

    //         if (httpCode > 0) {  // Check for the returning code
    //             String payload = http.getString();
    //             Serial.println(httpCode);
    //             Serial.println(payload);
    //         } else {
    //             Serial.println("Error on HTTP request");
    //         }
    //         http.end();  // Free the resources
    //     } else {
    //         printf("HTTP REQUEST FAILED, NOT CONNECTED TO THE INTERNET\n");
    //     }
    // }
};

#endif