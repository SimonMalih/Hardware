#ifndef GLOBAL_SETTINGS_H_
#define GLOBAL_SETTINGS_H_

#include <Arduino.h>
#include <HTTPClient.h>

#include <iostream>
#include <sstream>
#include <vector>

#include "Database.h"

using namespace std;

const int globalLength = 4;

class GlobalSettings {
   private:
    vector<string> splitStrings;
    bool settings[4] = {true, true, true, true};  // email, sms, telegram, push
    string userId = "Default";
    string number = "Default";
    string emailAdr = "Default";
    string name = "Default";
    enum RequestType { NOTIFICATION,
                       EMAIL,
                       SMS };

    void updateSettings(string s) {
        for (int i = 0; i < 4; i++) {
            settings[i] = s[i] == '1' ? true : false;
        }
    }

    void printSettings() {
        printf("Id: %s\n", userId.c_str());
        printf("Pin: %s\n", pin.c_str());
        printf("Name: %s\n", name.c_str());
        printf("Email: %s\n", emailAdr.c_str());
        printf("Number: %s\n", number.c_str());
        printf("Preferences: ");
        for (int i = 0; i < globalLength; i++) {
            printf("%s,", settings[i] ? "true" : "false");
        }
        printf("\n");
    }

    String getRandomString() {
        srand((unsigned)time(NULL));
        return to_string(rand()).c_str();
    }

    void makeRequest(String url) {
        if (WiFi.status() != WL_CONNECTED) {
            printf("Wifi is not connected, http request failed!\n");
            return;
        }

        HTTPClient http;
        http.begin(url);
        int httpCode = http.GET();  
        printf("Network request: %s\n", url.c_str());
        if (httpCode > 0) {
            printf("HTTP request failed with code: %d\n", &httpCode);
        }
        http.end();  // Free the resources
        return;
    }

    String buildURL(String type) {
        String id = userId.c_str();
        return "https://us-central1-iothome-a8984.cloudfunctions.net/" + type + "?uid=" + id + "&";
    }

    void sendSMS(bool success) {
        String function = success ? "smsAuthorizedAccess" : "smsUnauthorizedAccess";
        String base = buildURL(function);
        String numberStr = number.c_str();
        String nameStr = name.c_str();
        String appendExtra = "name=" + nameStr + "&number=" + numberStr;
        String url = base + appendExtra + "&randomESP32value=" + getRandomString();
        makeRequest(url);
    }

    void sendEmail(bool success) {
        String function = success ? "emailAutorizedAccess" : "emailUnautorizedAccess";
        String base = buildURL(function);
        String emailStr = emailAdr.c_str();
        String nameStr = name.c_str();
        String appendExtra = "name=" + nameStr + "&email=" + emailStr;
        String url = base + appendExtra + "&randomESP32value=" + getRandomString();
        makeRequest(url);
    }

   public:
    string pin = "9987";

    void updateUser(string settings) {
        vector<string> result;
        stringstream s_stream(settings);  // create string stream from the string

        while (s_stream.good()) {
            string substr;
            getline(s_stream, substr, '|');  // get first string delimited by comma
            result.push_back(substr);
        }

        // BkSsPV1qKCMj2JkiadCcZd0q1vO2|Simon|simon.malih@gmail.com|07482831705|1111|2222
        if (result.size() >= 6) {
            userId = result[0];
            name = result[1];
            emailAdr = result[2];
            number = result[3];
            updateSettings(result[4]);
            pin = result[5];
            printSettings();
        } else {
            printf("Failed to split strings for global user settings");
        }
    }

    

    void sendMessage(bool success) {
        String base = buildURL("writeNotification");
        String type = success ? "type=entry" : "type=intruder";
        String url = base + type + "&randomESP32value=" + getRandomString();
        makeRequest(url);

        if (settings[0])
            sendEmail(success);

        // if (settings[1])
        //     sendSMS(success);
    }


};

#endif