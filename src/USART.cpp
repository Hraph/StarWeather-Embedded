#include "USART.hpp"
#include "Monitor.hpp"

namespace Communication {
    void USART::initialize(){
        UBRR0H = (unsigned char)(UBRR_VALUE >> 8);
        UBRR0L = (unsigned char)UBRR_VALUE;
        UCSR0C = (0b011 << UCSZ00);//  8data, 1stop (default)
        UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // Activate Send / Receive with IRQ

        fdevopen(send, nullptr);
    }

    void USART::send(char data){
        while (!(UCSR0A & (1 << UDRE0))); // Wait for empty send buffer
	    UDR0 = data; // Write byte to send
    }

    void USART::debug(const char *data){
        printf("%c%s", COMMAND_DEBUG, data);
    }

    int USART::send(char data, FILE*){
        send(data);
	    return 0;
    }

    CircularBuffer<unsigned char, 2, unsigned char> USART::receiveBuffer; // Empty buffer

    ISR(USART_RX_vect){
        // Check for error
        if((UCSR0A & ((1 << FE0) | (1 << DOR0) | (1 << UPE0))) == 0) // Check frame error / data over run / parity error
        {
            USART::receiveBuffer.push(UDR0); // Save data
        }
    }
}