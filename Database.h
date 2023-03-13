#ifndef DATABASE_H_
#define DATABASE_H_

#include <Firebase_ESP_Client.h>

#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "Arduino.h"
#include "CustomAlarm.h"
#include "Device.h"
#include "Formatter.h"
#include "GlobalSettings.h"
#include "addons/RTDBHelper.h"
#include "addons/TokenHelper.h"
#include "AuthManager.h"

using namespace std;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool taskCompleted = false;
unsigned long dataMillis = 0;

class Database {
   public:
    const char *API_KEY = "AIzaSyCQ8uFVRzie9Na4t_aTGpgaFmXk1feMaq0";
    const char *PROJECT_ID = "iothome-a8984";
    const char *DATABASE_URL = "https://console.firebase.google.com/u/0/project/iothome-a8984/firestore/data/~2F";

    unsigned long sendDataPrevMillis = 0;
    int count = 0;
    bool signupOK = false;
    String base = "Devices/";

    Database() {
        config.api_key = API_KEY;
        config.database_url = DATABASE_URL;

        if (Firebase.signUp(&config, &auth, "", "")) {
            Serial.println("\nSUCCESSFULLY CONNECTED TO FIREBASE DATABASE");
            signupOK = true;
        } else {
            Serial.printf("%s\n", config.signer.signupError.message.c_str());
        }
    }

    Database(const Database &) { initDatabase(); }

    void initDatabase() {
        config.api_key = API_KEY;
        config.database_url = DATABASE_URL;

        if (Firebase.signUp(&config, &auth, "", "")) {
            Serial.println("ok");
            signupOK = true;
        } else {
            Serial.printf("%s\n", config.signer.signupError.message.c_str());
        }

        config.token_status_callback = tokenStatusCallback;
        Firebase.begin(&config, &auth);
        Firebase.reconnectWiFi(true);
    }

    bool isReady() {
        if (!Firebase.ready()) {
            Serial.printf("Firebase was not ready!");
            return false;
        } else if (WiFi.status() != WL_CONNECTED) {
            Serial.printf("ESP32 is not connected to the wifi\n");
            // reconnect wifi

            return false;
        }
        return true;
    }

    void write(Device &device) {
        if (!isReady()) {
            return;
        }

        string documentPath = "Devices/" + device.getPosition();
        FirebaseJson content;
        string path = "fields/value/" + device.getDataType();
        content.set(path, String(device.getValue()));

        if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", documentPath.c_str(), content.raw(), "value")) {
            // Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            return;
        } else if (Firebase.Firestore.createDocument(&fbdo, PROJECT_ID, "", documentPath.c_str(), content.raw())) {
            Serial.printf("Successfully created firebase database document\n");
            // Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            return;
        } else {
            Serial.println(fbdo.errorReason());
        }
    }

    void read(Device &device) {
        if (!isReady()) {
            return;
        }

        // attempt to create document if it doesnt exist
        string documentPath = "Devices/" + device.getPosition();
        FirebaseJson content;
        string path = "fields/value/" + device.getDataType();
        content.set(path, String(device.getValue()).c_str());
        if (Firebase.Firestore.createDocument(&fbdo, PROJECT_ID, "", documentPath.c_str(), content.raw())) {
            Serial.println("Read operation postponed to create document!");
            return;
        }

        if (Firebase.Firestore.getDocument(&fbdo, PROJECT_ID, "", documentPath.c_str(), "")) {
            // Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            string type = device.getDataType();
            int val = Formatter::filterInt(fbdo.payload().c_str(), device.getDataType());
            device.setValue(val);
        } else {
            Serial.println(fbdo.errorReason());
        }
    }

    void readSettings(GlobalSettings &settings) {
        if (!isReady()) {
            return;
        }

        string documentPath = "CurrentUser/Settings";
        FirebaseJson content;
        string path = "fields/value/string";
        content.set(path, "");
        if (Firebase.Firestore.createDocument(&fbdo, PROJECT_ID, "", documentPath.c_str(), content.raw())) {
            Serial.println("Read operation postponed to create document!");
            return;
        }

        if (Firebase.Firestore.getDocument(&fbdo, PROJECT_ID, "", documentPath.c_str(), "")) {
            settings.updateSettings(Formatter::filterString(fbdo.payload().c_str(), "stringValue"));
            settings.printSettings();
        } else {
            Serial.println(fbdo.errorReason());
        }
    }

    void readUser(GlobalSettings &settings) {
        if (!isReady()) {
            return;
        }

        string documentPath = "CurrentUser/CurrentUser";
        FirebaseJson content;
        string path = "fields/value/string";
        content.set(path, "");
        if (Firebase.Firestore.createDocument(&fbdo, PROJECT_ID, "", documentPath.c_str(), content.raw())) {
            Serial.println("Read operation postponed to create document!");
            return;
        }

        if (Firebase.Firestore.getDocument(&fbdo, PROJECT_ID, "", documentPath.c_str(), "")) {
            settings.updateID(Formatter::filterSingleString(fbdo.payload().c_str()));
            //printf("%s\n", fbdo.payload().c_str());
        } else {
            Serial.println(fbdo.errorReason());
        }
    }

    void readPin(AuthManager &authManager) {
        if (!isReady()) {
            return;
        }
        string documentPath = "CurrentUser/Pin";
        FirebaseJson content;
        string path = "fields/value/string";
        content.set(path, "");
        if (Firebase.Firestore.createDocument(&fbdo, PROJECT_ID, "", documentPath.c_str(), content.raw())) {
            Serial.println("Read operation postponed to create document!");
            return;
        }

        if (Firebase.Firestore.getDocument(&fbdo, PROJECT_ID, "", documentPath.c_str(), "")) {
            authManager.setPin(Formatter::filterSingleString(fbdo.payload().c_str()));
        } else {
            Serial.println(fbdo.errorReason());
        }
    }

    void writeAlarm(CustomAlarm alarm) {
        if (!isReady()) {
            return;
        }

        string documentPath = "CurrentUser/Pin";
        FirebaseJson content;
        string path = "fields/value/integerValue";
        content.set(path, (alarm.auth ? "1" : "0"));

        if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", documentPath.c_str(), content.raw(), "value")) {
            return;
        } else if (Firebase.Firestore.createDocument(&fbdo, PROJECT_ID, "", documentPath.c_str(), content.raw())) {
            Serial.printf("Successfully created firebase database document\n");
            return;
        } else {
            Serial.println(fbdo.errorReason());
        }
    }

    void write(int value) {
        string documentPath = "Sensor/Steps";
        FirebaseJson content;
        string path = "fields/value/integerValue";  // integerValue or stringValue
        content.set(path, String(value));

        if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", documentPath.c_str(), content.raw(), "value")) {
            // Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            return;
        } else if (Firebase.Firestore.createDocument(&fbdo, PROJECT_ID, "", documentPath.c_str(), content.raw())) {
            Serial.printf("Successfully created firebase database document\n");
            // Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            return;
        } else {
            Serial.println(fbdo.errorReason());
        }
    }
};

#endif
