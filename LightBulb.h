#pragma once

#include <iostream>
#include "Device.h"

class Lightbulb : public Device {
   public:
    Lightbulb(std::string position) : Device(position, Device::LIGHTBULB) {}

    bool getBool() {
        return !(intValue == 0);
    }
};

