#include "App.hpp"

namespace Application {
    Config App::config = Config();

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

        printf("/nconfig saved/n");
    }
}