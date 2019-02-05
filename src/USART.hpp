#ifndef USART_H
#define USART_H

#include <Arduino.h>

#define BAUD 250000
#define UBRR_VALUE ((F_CPU/16/BAUD)-1)

#pragma once

namespace Communication {
    class USART {
    public:
        static void initialize();
        static unsigned char receivedBuffer;
    private:
        inline static void send(char data);
        inline static int send(char data, FILE*); 
    };
}

#endif //USART.hpp