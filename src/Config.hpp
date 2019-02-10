#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG
//#define DEBUG_COMMANDS

#define CACHEBUFFER_CAPACITY 360

/**
 * Sensors configuration
 */

#define SENSORS_COUNT 3
#define SENSOR_ADDRESS_A 1
#define SENSOR_ADDRESS_B 2
#define SENSOR_ADDRESS_C 3

/**
 * Memory configuration
 */

#define MEMORY_INITIALIZED_CONTENT 123

#define MEMORY_ADDRESS_INITIALIZEDMEMORY 0
#define MEMORY_ADDRESS_MODE 1
#define MEMORY_ADDRESS_FREQUENCYSENSOR_A 2
#define MEMORY_ADDRESS_FREQUENCYSENSOR_B 3
#define MEMORY_ADDRESS_FREQUENCYSENSOR_C 4
#define MEMORY_ADDRESS_DELAY_MODE_2 5

/**
 * LED Configuration
 */

#define LEDG 5
#define LEDR 6
#define LEDB 7

#pragma once

namespace Application {
    class Config {
        public:
            uint8_t mode = 0;
            uint8_t frequencySensor_A = 1;
            uint8_t frequencySensor_B = 1;
            uint8_t frequencySensor_C = 1;
            uint8_t delay_Mode_2 = 1;
    };
}

#endif //Config.hpp