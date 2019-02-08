#include "Analog.hpp"

#define MUXn_MASK 0b1111 // Mask to get the selected MUX channel

namespace IO {
    void Analog::initialize(){
        //Initialize ADC
        ADCSRA = (1 << ADEN) | (0b101 << ADPS0); // Enable and set prescaler division by 32
        ADCSRA |= (1 << ADIE); // Enable interrupt on end conversion
    }

    ISR(ADC_vect) {
        //Get measure of current MUX
        switch(MUXn_MASK & ADMUX){
            case SENSOR_ADDRESS_A:
                Analog::values[0] = ADCL; // Left
                Analog::values[0] |= ((uint16_t)ADCH) << 8; // Right

                Analog::updatedValueFlags[0] = true;
                Analog::checkValueFlags[0] = false;
            break;
            case SENSOR_ADDRESS_B:
                Analog::values[1] = ADCL; // Left
                Analog::values[1] |= ((uint16_t)ADCH) << 8; // Right

                Analog::updatedValueFlags[1] = true;
                Analog::checkValueFlags[1] = false;
            break;
            case SENSOR_ADDRESS_C:
                Analog::values[2] = ADCL; // Left
                Analog::values[2] |= ((uint16_t)ADCH) << 8; // Right

                Analog::updatedValueFlags[2] = true;
                Analog::checkValueFlags[2] = false;
            break;
        }

        // Ask for next measure
        if (Analog::checkValueFlags[0]) { // Sensor_A
            ADMUX = (0b01 << REFS0) | SENSOR_ADDRESS_A; // Change multiplexer channel
            ADCSRA |= (1 << ADSC); // Launch ADC converter
        }
        if (Analog::checkValueFlags[1]) { // Sensor_B
            ADMUX = (0b01 << REFS0) | SENSOR_ADDRESS_B; // Change multiplexer channel
            ADCSRA |= (1 << ADSC); // Launch ADC converter
        }
        if (Analog::checkValueFlags[2]) { // Sensor_C
            ADMUX = (0b01 << REFS0) | SENSOR_ADDRESS_C; // Change multiplexer channel
            ADCSRA |= (1 << ADSC); // Launch ADC converter
        }
    }

    unsigned int Analog::values[SENSORS_COUNT] = {0};
    bool Analog::updatedValueFlags[SENSORS_COUNT] = {false}; 
    bool Analog::checkValueFlags[SENSORS_COUNT] = {false}; 
}