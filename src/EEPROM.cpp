#include "EEPROM.hpp"

namespace Memory {
    void EEPROM::write(unsigned int address, unsigned char data) {
        while(EECR & (1 << EEPE)); // Wait until become 0
        EEAR = address; // Set the data address
        EEDR = data; // EERPOM write data
        EECR |= 1 << EEMPE; // Control register 
        EECR |= 1 << EEPE; // Enable write
    }


    unsigned char EEPROM::read(unsigned int address) {
        while (EECR & (1 << EEPE)); // Wait until become 0
        EEAR = address; // Set the data address
        EECR |= (1 << EERE); // Enable read
        return EEDR; // Get value
    }
}