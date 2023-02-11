#ifndef LIGHTBULB_H_
#define LIGHTBULB_H_

#include <iostream>

#include "Device.h"

class Lightbulb : public Device {
   public:
    Lightbulb(std::string position) : Device(position, Device::LIGHTBULB) {}
};

#endif