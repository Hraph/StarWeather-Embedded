//Using ATmega328P

#include<avr/interrupt.h>
#include "App.hpp"
#include "USART.hpp"
#include "Config.hpp"
#include "Analog.hpp"
#include "Monitor.hpp"

using namespace Application;
using namespace Communication;
using namespace Memory;
using namespace IO;

inline void printAllAnalogCachedBuffer(uint8_t command){
	if (!Analog::cacheBuffer.isEmpty()) {
		USART::send(command);
		uint16_t size = Analog::cacheBuffer.size();
		USART::sendInt(size);

		for(uint16_t i = 0; i < size; ++i){
			uint32_t stored = Analog::cacheBuffer.shift();

			USART::sendInt32((App::timestamp & ASSEMBLE_SHORT_TIMESTAMP_MASK) | ((stored & ASSEMBLE_SHORT_TIMESTAMP_MASK) >> 16)); // Assemble timestamp with splitted shortTimestamp 
			USART::sendInt(uint16_t(stored & SHORT_TIMESTAMP_MASK));
		}
	}
}

uint32_t lastCheckedTimestampMode2 = 0;

int main(){
	USART::initialize();
	App::initialize();
	Analog::initialize();

	#ifdef DEBUG_COMMANDS
		printf("USART Initialized - Ready !\n");
	#endif // DEBUG (DEBUG)

	App::loadConfig();
	#ifdef DEBUG_COMMANDS
		printf("MODE:%d - F1: %d - F2: %d -  F3: %d - DELAY MODE 2: %d\n", App::config.mode, App::config.frequencySensor_A, App::config.frequencySensor_B, App::config.frequencySensor_C, App::config.delay_Mode_2);
	#endif // DEBUG
	
	App::updateLEDModeColor(); // Launch check

	sei(); // Allow interrupt

	// Loop
	while (1){
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

		// Mode 2
		else if (App::config.mode == APP_MODE_2 && (App::timestamp % (App::config.delay_Mode_2 * 1000) == 0) && App::timestamp != lastCheckedTimestampMode2){ // Send data on each delay once
			lastCheckedTimestampMode2 = App::timestamp;
			printAllAnalogCachedBuffer(COMMAND_SEND_MODE2_DATA);
		}

		// Mode 3
		else if (App::config.mode == APP_MODE_3 && App::sendAllDataFlag){
			App::sendAllDataFlag = false;
			printAllAnalogCachedBuffer(COMMAND_GET_DATA);
		}
	}
}