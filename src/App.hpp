#ifndef APP_H
#define APP_H

#include <Arduino.h>

#include "Config.hpp"
#include "EEPROM.hpp"

#define SENSORS_COUNT 3
#define SENSOR_ADDRESS_A 1
#define SENSOR_ADDRESS_B 2
#define SENSOR_ADDRESS_C 3

#pragma once

namespace Application {
    class App {
    public:
        static void initialize();
        static Config config;
        static void loadConfig();
        static void saveConfig();
        static volatile unsigned int timestamp;
    private:
    };
}

#endif //App.hpp