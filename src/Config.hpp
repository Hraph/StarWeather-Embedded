#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define SENSORS_COUNT 3
#define SENSOR_ADDRESS_A 1
#define SENSOR_ADDRESS_B 2
#define SENSOR_ADDRESS_C 3

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

namespace Application {
    class Config {
        public:
            uint8_t mode = 0;
            uint8_t frequencySensor_A = 1;
            uint8_t frequencySensor_B = 1;
            uint8_t frequencySensor_C = 1;
            uint8_t delay_Mode_2 = 0;
    };
}

#endif //Config.hpp