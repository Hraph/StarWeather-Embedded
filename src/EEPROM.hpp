#ifndef EEPROM_H
#define EEPROM_H

#include <avr/io.h>

#pragma once

namespace Memory {
    class EEPROM {
    public:
        static void write(unsigned int address, unsigned char data);
        static unsigned char read(unsigned int address);
    private:
    };
}

#endif //EEPROM.hpp