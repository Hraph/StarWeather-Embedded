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

    ISR(TIMER1_OVF_vect) {
        App::timestamp++;

        if (App::timestamp % int(1000 / App::config.frequencySensor_A) == 0) {
            ADMUX = (0b01 << REFS0) | SENSOR_ADDRESS_A; // Change multiplexer channel
            ADCSRA |= (1 << ADSC); // Launch ADC converter
        }
        if (App::timestamp % int(1000 / App::config.frequencySensor_B) == 0) {
            ADMUX = (0b01 << REFS0) | SENSOR_ADDRESS_B; // Change multiplexer channel
            ADCSRA |= (1 << ADSC); // Launch ADC converter
        }
        if (App::timestamp % int(1000 / App::config.frequencySensor_C) == 0) {
            ADMUX = (0b01 << REFS0) | SENSOR_ADDRESS_C; // Change multiplexer channel
            ADCSRA |= (1 << ADSC); // Launch ADC converter
        }

        TCNT1 = 65535 - 16000; // 1ms
    }

    void App::loadConfig(){
        if (Memory::EEPROM::read(Memory::ConfigAddresses::InitializedMemory) == MEMORY_INITIALIZED_CONTENT) { // Check if config has already been saved
            App::config.mode = Memory::EEPROM::read(Memory::ConfigAddresses::Mode); // Mode
            App::config.frequencySensor_A = Memory::EEPROM::read(Memory::ConfigAddresses::FrequencySensor_A); // Frequency A
            App::config.frequencySensor_B = Memory::EEPROM::read(Memory::ConfigAddresses::FrequencySensor_B); // Frequency B
            App::config.frequencySensor_C = Memory::EEPROM::read(Memory::ConfigAddresses::FrequencySensor_C); // Frequency C
            App::config.delay_Mode_2 = Memory::EEPROM::read(Memory::ConfigAddresses::Delay_Mode_2); // Delay Mode 2
        }
    }

    void App::saveConfig(){
        Memory::EEPROM::write(Memory::ConfigAddresses::InitializedMemory, MEMORY_INITIALIZED_CONTENT); // Set initialized flag in memory

        Memory::EEPROM::write(Memory::ConfigAddresses::Mode, App::config.mode); // Mode
        Memory::EEPROM::write(Memory::ConfigAddresses::FrequencySensor_A, App::config.frequencySensor_A); // Frequency A
        Memory::EEPROM::write(Memory::ConfigAddresses::FrequencySensor_B, App::config.frequencySensor_B); // Frequency B
        Memory::EEPROM::write(Memory::ConfigAddresses::FrequencySensor_C, App::config.frequencySensor_C); // Frequency C
        Memory::EEPROM::write(Memory::ConfigAddresses::Delay_Mode_2, App::config.delay_Mode_2); // Delay Mode 2

        printf("\nconfig saved\n");
    }

    volatile unsigned int App::timestamp = 0;
}