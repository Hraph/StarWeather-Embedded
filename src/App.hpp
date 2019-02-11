#ifndef APP_H
#define APP_H

#include "avr/io.h"
#include "stdint.h"
#include "Config.hpp"
#include "EEPROM.hpp"
#include "Analog.hpp"
#include "USART.hpp"

#define APP_MODE_STOP 0
#define APP_MODE_1 1
#define APP_MODE_2 2
#define APP_MODE_3 3

#define MSK_LEDR (1 << LEDR)
#define MSK_LEDG (1 << LEDG)
#define MSK_LEDB (1 << LEDB)

#pragma once

namespace Application {
    class App {
    public:
        static void initialize();
        static Config config;
        static void loadConfig();
        static void saveConfig();
        static volatile uint32_t timestamp;
        static bool sendAllDataFlag;
        static void updateLEDModeColor();
    private:
    };
}

#endif //App.hpp