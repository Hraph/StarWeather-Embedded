#ifndef ANALOG_H
#define ANALOG_H

#include "avr/io.h"
#include "avr/interrupt.h"
#include "stdint.h"
#include "Config.hpp"
#include "App.hpp"
#include "CircularBuffer.hpp"

#define SHORT_TIMESTAMP_MASK 0xFFFF
#define ASSEMBLE_SHORT_TIMESTAMP_MASK 0xFFFF0000

#pragma once

namespace IO {
    class Analog {
    public:
        static void initialize();
        static unsigned int values[SENSORS_COUNT];
        static bool updatedValueFlags[SENSORS_COUNT];
        static bool checkValueFlags[SENSORS_COUNT];
        static CircularBuffer<uint32_t, 300> cacheBuffer;

        // Inline functions must be defined in header to be recognized by the linker
        static inline uint16_t concatValueWithSensor(uint16_t value, uint8_t sensor) {
            return (value | (sensor << 10)); // Add sensor bits after 10bits of ADC value
        }
    private:
    };
}

#endif //Analog.hpp