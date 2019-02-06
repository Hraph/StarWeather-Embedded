#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define MEMORY_INITIALIZED_CONTENT 123

#pragma once

namespace Memory {
    enum ConfigAddresses {
        InitializedMemory = 0,
        Mode,
        FrequencySensor_A,
        FrequencySensor_B,
        FrequencySensor_C,
        Delay_Mode_2
    };
}

namespace App {
    class Config {
        public:
            uint8_t mode = 0;
            uint8_t frequencySensor_A = 0;
            uint8_t frequencySensor_B = 0;
            uint8_t frequencySensor_C = 0;
            uint8_t delay_Mode_2 = 0;
    };
}

#endif //Config.hpp