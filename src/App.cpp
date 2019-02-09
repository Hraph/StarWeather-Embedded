#include "App.hpp"

namespace Application {
    Config App::config = Config();

    void App::initialize(){
        // Initialize timer

        // MODE 4 	WMG13	WMG2	WMG11	WMG10
	    //			0		1		0		0
        TCCR1A = (0b00 << WGM10); // Use mode 4
        TCCR1B = (1 << WGM12) | (1 << CS10); // Prescaler of 1
        TCNT1 = 65535 - 16000; // 1ms
        TIMSK1 = (1 << TOIE1); // Activate interrupts
    }

    // 1ms tick timer
    ISR(TIMER1_OVF_vect) {
        App::timestamp++;

        // Frequency can be the same for multiple sensors
        if (App::timestamp % int(1000 / App::config.frequencySensor_A) == 0) {
            IO::Analog::checkValueFlags[0] = true; // Ask for a new value
            
            ADMUX = (0b01 << REFS0) | SENSOR_ADDRESS_A; // Change multiplexer channel
            ADCSRA |= (1 << ADSC); // Launch ADC converter
        }
        if (App::timestamp % int(1000 / App::config.frequencySensor_B) == 0) {
            IO::Analog::checkValueFlags[1] = true; // Ask for a new value

            ADMUX = (0b01 << REFS0) | SENSOR_ADDRESS_B; // Change multiplexer channel
            ADCSRA |= (1 << ADSC); // Launch ADC converter
        }
        if (App::timestamp % int(1000 / App::config.frequencySensor_C) == 0) {
            IO::Analog::checkValueFlags[1] = true; // Ask for a new value
            
            ADMUX = (0b01 << REFS0) | SENSOR_ADDRESS_C; // Change multiplexer channel
            ADCSRA |= (1 << ADSC); // Launch ADC converter
        }

        TCNT1 = 65535 - 16000; // 1ms
    }

    void App::loadConfig(){
        if (Memory::EEPROM::read(MEMORY_ADDRESS_INITIALIZEDMEMORY) == MEMORY_INITIALIZED_CONTENT) { // Check if config has already been saved
            config.mode = Memory::EEPROM::read(MEMORY_ADDRESS_MODE); // Mode
            config.frequencySensor_A = Memory::EEPROM::read(MEMORY_ADDRESS_FREQUENCYSENSOR_A); // Frequency A
            config.frequencySensor_B = Memory::EEPROM::read(MEMORY_ADDRESS_FREQUENCYSENSOR_B); // Frequency B
            config.frequencySensor_C = Memory::EEPROM::read(MEMORY_ADDRESS_FREQUENCYSENSOR_C); // Frequency C
            config.delay_Mode_2 = Memory::EEPROM::read(MEMORY_ADDRESS_DELAY_MODE_2); // Delay Mode 2

            #ifdef DEBUG_COMMANDS
                printf("\nConfiguration read from EEPROM\n");
            #endif // DEBUG
        }
    }

    void App::saveConfig(){
        Memory::EEPROM::write(MEMORY_ADDRESS_INITIALIZEDMEMORY, MEMORY_INITIALIZED_CONTENT); // Set initialized flag in memory

        Memory::EEPROM::write(MEMORY_ADDRESS_MODE, config.mode); // Mode
        Memory::EEPROM::write(MEMORY_ADDRESS_FREQUENCYSENSOR_A, config.frequencySensor_A); // Frequency A
        Memory::EEPROM::write(MEMORY_ADDRESS_FREQUENCYSENSOR_B, config.frequencySensor_B); // Frequency B
        Memory::EEPROM::write(MEMORY_ADDRESS_FREQUENCYSENSOR_C, config.frequencySensor_C); // Frequency C
        Memory::EEPROM::write(MEMORY_ADDRESS_DELAY_MODE_2, config.delay_Mode_2); // Delay Mode 2

        #ifdef DEBUG_COMMANDS
            printf("\nConfiguration saved\n");
        #endif // DEBUG
    }

    volatile uint32_t App::timestamp = 0;
}