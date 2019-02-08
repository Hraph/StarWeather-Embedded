#ifndef USART_H
#define USART_H

#include <Arduino.h>
#include "CircularBuffer.hpp"

#define BAUD 250000
#define UBRR_VALUE ((F_CPU/16/BAUD)-1)

#define NULL_CHARACTER '\0'

#pragma once

namespace Communication {
    class USART {
    public:
        static void initialize();
        static CircularBuffer<unsigned char, 2> receiveBuffer;
        inline static void send(char data);
        static void debug(const char *data);
    private:
        inline static int send(char data, FILE*); 
    };
}

#endif //USART.hpp