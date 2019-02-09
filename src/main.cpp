//Using ATmega328P
#include<avr/interrupt.h>

//#define DEBUG
//#define DEBUG_COMMANDS

#include "App.hpp"
#include "USART.hpp"
#include "Config.hpp"
#include "Analog.hpp"
#include "Monitor.hpp"

using namespace Application;
using namespace Communication;
using namespace Memory;
using namespace IO;

void setup() {
	USART::initialize();
	App::initialize();
	Analog::initialize();

	#ifdef DEBUG
		printf("USART Initialized - Ready !\n");
	#endif // DEBUG (DEBUG)

	App::loadConfig();
	#ifdef DEBUG
		printf("MODE:%d - F1: %d - F2: %d -  F3: %d - DELAY MODE 2: %d\n", App::config.mode, App::config.frequencySensor_A, App::config.frequencySensor_B, App::config.frequencySensor_C, App::config.delay_Mode_2);
	#endif // DEBUG

	sei(); // Allow interrupt
}

void loop() {
	// Mode 1
	if (App::config.mode == APP_MODE_1){
		if (Analog::updatedValueFlags[0]) {
			Analog::updatedValueFlags[0] = false;
			USART::send(COMMAND_SEND_MODE1_DATA);
			USART::sendInt32(App::timestamp);
			USART::sendInt(Analog::concatValueWithSensor(Analog::values[0], SENSOR_ADDRESS_A));
		}
		if (Analog::updatedValueFlags[1]) {
			Analog::updatedValueFlags[1] = false;
			USART::send(COMMAND_SEND_MODE1_DATA);
			USART::sendInt32(App::timestamp);
			USART::sendInt(Analog::concatValueWithSensor(Analog::values[1], SENSOR_ADDRESS_B));	
		}
		if (Analog::updatedValueFlags[2]) {
			Analog::updatedValueFlags[2] = false;
			USART::send(COMMAND_SEND_MODE1_DATA);
			USART::sendInt32(App::timestamp);
			USART::sendInt(Analog::concatValueWithSensor(Analog::values[2], SENSOR_ADDRESS_C));
		}
	}
		}
	}
}