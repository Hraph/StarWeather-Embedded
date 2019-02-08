#ifndef ANALOG_H
#define ANALOG_H

#include <Arduino.h>
#include "Config.hpp"

#pragma once

namespace IO {
    class Analog {
    public:
        static void initialize();
        static unsigned int values[SENSORS_COUNT];
        static bool updatedValueFlags[SENSORS_COUNT];
        static bool checkValueFlags[SENSORS_COUNT];
    private:
    };
}

#endif //Analog.hpp