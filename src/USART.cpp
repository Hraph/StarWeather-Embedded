#include "USART.hpp"

namespace Communication {
    void USART::initialize(){
        UBRR0H = (unsigned char)(UBRR_VALUE >> 8);
        UBRR0L = (unsigned char)UBRR_VALUE;
        UCSR0C = (0b011 << UCSZ00);//  8data, 1stop (default)
        UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // Activate Send / Receive with IRQ

        fdevopen(send, nullptr);
    }

    int USART::send(char data, FILE*){
        send(data);
	    return 0;
    }

    uint8_t USART::waitForNextCharReceived(){
        while(!(UCSR0A & (1 << RXC0)));
        if((UCSR0A & ((1 << FE0) | (1 << DOR0) | (1 << UPE0))) == 0) // Check frame error / data over run / parity error
        {
            return UDR0;
        }
        return '\0';
    }

}