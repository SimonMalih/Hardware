#ifndef GLOBAL_SETTINGS_H_
#define GLOBAL_SETTINGS_H_

#include <Arduino.h>

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

const int globalLength = 4;

class GlobalSettings {
   public:
    bool email, sms, push;
    vector<string> splitStrings;
    bool settings[4] = {true, true, true, true};
    string userId = "Default";

    void updateSettings(string s) {
        for (int i = 0; i < 4; i++) {
            settings[i] = s[i] == '1' ? true : false;
        }
    }

    void updateID(string id) {
        userId = id;
    }

    void printSettings() {
        for (int i = 0; i < globalLength; i++) {
            printf("%s\n", settings[i] ? "true" : "false");
        }
    }
};

#endif