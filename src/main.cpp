//ATmega328P
#include<Arduino.h>

#include "App.hpp"
#include "USART.hpp"
#include "EEPROM.hpp"
#include "Analog.hpp"

using namespace Application;
using namespace Communication;
using namespace Memory;
using namespace IO;

void setup() {
	USART::initialize();
	App::initialize();
	Analog::initialize();

	printf("USART Initialized - Ready !\n");
	App::loadConfig();
	printf("MODE:%d - F1: %d - F2: %d -  F3: %d - DELAY MODE 2: %d\n", App::config.mode, App::config.frequencySensor_A, App::config.frequencySensor_B, App::config.frequencySensor_C, App::config.delay_Mode_2);
	sei(); // Allow interrupt
}

void loop() {
	if (Analog::updatedValueFlags[0]) {
		Analog::updatedValueFlags[0] = false;
		printf("update A");
	}
	if (Analog::updatedValueFlags[1]) {
		Analog::updatedValueFlags[1] = false;
		printf("update B");
	}
	if (Analog::updatedValueFlags[2]) {
		Analog::updatedValueFlags[2] = false;
		printf("update C");
	}
}