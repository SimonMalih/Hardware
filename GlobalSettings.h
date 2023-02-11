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

    void updateSettings(string s) {
        for (int i = 0; i < 4; i++) {
            settings[i] = s[i] == '1' ? true : false;
        }
    }

    void printSettings() {
        for (int i = 0; i < globalLength; i++) {
            //printf("%s\n", settings[i] ? "true" : "false");
        }
    }

    // vector<string> split(const string &s, char delim) {
    //     vector<string> result;
    //     stringstream ss(s);
    //     string item;

    //     while (getline(ss, item, delim)) {
    //         result.push_back(item);
    //     }

    //     return result;
    // }
};

#endif