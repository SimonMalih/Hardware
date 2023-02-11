#ifndef DEVICE_H_
#define DEVICE_H_

#include <iostream>

#include "Formatter.h"

class Device {
   public:
    enum Type {
        LIGHTBULB,
        THERMOMETER,
        HUMIDITY,
        LIGHTSENSOR,
        SPEAKER
    };

    Type type;
    std::string position;

    //float floatValue;
    int intValue = 3;

    std::string getDataType(){
        return "integerValue";
    }

    std::string getName() {
        switch (type) {
            case LIGHTBULB:
                return "LIGHTBULB";

            case THERMOMETER:
                return "THERMOMETER";

            case HUMIDITY:
                return "HUMIDITY";

            case LIGHTSENSOR:
                return "LIGHTSENSOR";

            case SPEAKER:
                return "SPEAKER";

            default:
                return "LIGHTBULB";
        }
    }

    std::string getPosition() {
        return getName() + position + "/";
    }

    // void setValue(const float &n) {
    //     //floatValue = n;
    //     intValue = -1;
    // }

    virtual void setValue(const int &n) {
        //floatValue = -1.0f;
        intValue = n;
    }

    virtual String getValue() {
        std::string t = getDataType();
        // if (t.compare("doubleValue") == 0) {
        //     return String(floatValue, 2);
        // } else {
        //     return String(intValue);
        // }
        return String(intValue);
    }

    Device(std::string position, Type type) {
        this->position = position;
        this->type = type;
    }

    Device(std::string position) {
        this->position = position;
        this->type = LIGHTBULB;
    }
};
#endif