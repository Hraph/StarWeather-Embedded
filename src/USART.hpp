#ifndef USART_H
#define USART_H

#include <avr/io.h>
#include <stdio.h>

#define BAUD 76800
#define UBRR_VALUE ((F_CPU/16/BAUD)-1)

#define NULL_CHARACTER '\0'

#pragma once

namespace Communication {
    class USART {
    public:
        static void initialize();
        static uint8_t waitForNextCharReceived();
        static void debug(const char *data);

        // Inline functions must be defined in header to be recognized by the linker
        static inline void sendInt32(int32_t n){
            USART::send((n & 0xff000000) >> 24);
            USART::send((n & 0x00ff0000) >> 16);
            USART::send((n & 0x0000ff00) >> 8);
            USART::send(n & 0x000000ff);
        }
        static inline void sendInt(int n){
            USART::send(n >> 8);
            USART::send(n & 0xFF);
        }
        static inline void send(char data){
            while (!(UCSR0A & (1 << UDRE0))); // Wait for empty send buffer
            UDR0 = data; // Write byte to send
        }
    private:
        static inline int send(char data, FILE*); 
    };  
}

#endif //USART.hpp