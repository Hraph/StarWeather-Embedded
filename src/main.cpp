//ATmega328P
#include<Arduino.h>

//#define DEBUG
//#define DEBUG_COMMANDS

#include "App.hpp"
#include "USART.hpp"
#include "EEPROM.hpp"
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
	App::config.frequencySensor_A = 10;
	App::config.frequencySensor_B = 15;
	App::config.frequencySensor_C = 20;
	App::config.mode = APP_MODE_STOP;
	#ifdef DEBUG
		printf("MODE:%d - F1: %d - F2: %d -  F3: %d - DELAY MODE 2: %d\n", App::config.mode, App::config.frequencySensor_A, App::config.frequencySensor_B, App::config.frequencySensor_C, App::config.delay_Mode_2);
	#endif // DEBUG

	sei(); // Allow interrupt
}

inline void waitForNewUSARTByte(){
	while(USART::receiveBuffer.isEmpty()) { //Wait for second byte
		sei();
	};
}

bool readUSARTAndUpdateFrequency(uint8_t &configFrequency){
	waitForNewUSARTByte();

	int freq = USART::receiveBuffer.shift();
	if (freq >= 1 && freq <= 20) {
		configFrequency = freq;
		App::saveConfig();
		return true;
	}
	return false;
}

inline int getValueWithSensor(int value, uint8_t sensor){
	return (value | (sensor << 10)); // Add sensor bits after 10bits of ADC value
}

void printuInt32(uint32_t n){
	USART::send((n & 0xff000000) >> 24);
	USART::send((n & 0x00ff0000) >> 16);
	USART::send((n & 0x0000ff00) >> 8);
	USART::send(n & 0x000000ff);
}
void printInt(int n){
	USART::send(n >> 8);
	USART::send(n & 0xFF);
}

void loop() {
	// Mode 1
	if (App::config.mode == APP_MODE_1){
		if (Analog::updatedValueFlags[0]) {
			Analog::updatedValueFlags[0] = false;
			printf("%c", COMMAND_SEND_MODE1_DATA);
			printuInt32(App::timestamp);
			printInt(getValueWithSensor(Analog::values[0], SENSOR_ADDRESS_A));
		}
		if (Analog::updatedValueFlags[1]) {
			Analog::updatedValueFlags[1] = false;
			printf("%c", COMMAND_SEND_MODE1_DATA);
			printuInt32(App::timestamp);
			printInt(getValueWithSensor(Analog::values[1], SENSOR_ADDRESS_B));	
		}
		if (Analog::updatedValueFlags[2]) {
			Analog::updatedValueFlags[2] = false;
			printf("%c", COMMAND_SEND_MODE1_DATA);
			printuInt32(App::timestamp);
			printInt(getValueWithSensor(Analog::values[2], SENSOR_ADDRESS_C));
		}
	}
	
	// Monitor Commands
	if (!USART::receiveBuffer.isEmpty()){
		switch(USART::receiveBuffer.shift()){
			case COMMAND_STOP_MODE:
				App::config.mode = APP_MODE_STOP;
				App::saveConfig();
				Monitor::sendCommandResponse(COMMAND_STOP_MODE, COMMAND_RESPONSE_SUCCESS);
				break;
			case COMMAND_START_MODE_1:
				App::config.mode = APP_MODE_1;
				App::saveConfig();
				Monitor::sendCommandResponse(COMMAND_START_MODE_1, COMMAND_RESPONSE_SUCCESS);
				break;
			case COMMAND_START_MODE_2:
				App::config.mode = APP_MODE_2;
				App::saveConfig();
				Monitor::sendCommandResponse(COMMAND_START_MODE_2, COMMAND_RESPONSE_SUCCESS);
				break;
			case COMMAND_START_MODE_3:
				App::config.mode = APP_MODE_3;
				App::saveConfig();
				Monitor::sendCommandResponse(COMMAND_START_MODE_3, COMMAND_RESPONSE_SUCCESS);
				break;
			case COMMAND_CONFIGURE_FE_1:
				if (readUSARTAndUpdateFrequency(App::config.frequencySensor_A))
					Monitor::sendCommandResponse(COMMAND_CONFIGURE_FE_1, COMMAND_RESPONSE_SUCCESS);
				else
					Monitor::sendCommandResponse(COMMAND_CONFIGURE_FE_1, COMMAND_RESPONSE_FAILURE);		 
				break;
			case COMMAND_CONFIGURE_FE_2:
				if (readUSARTAndUpdateFrequency(App::config.frequencySensor_B))
					Monitor::sendCommandResponse(COMMAND_CONFIGURE_FE_2, COMMAND_RESPONSE_SUCCESS);
				else
					Monitor::sendCommandResponse(COMMAND_CONFIGURE_FE_2, COMMAND_RESPONSE_FAILURE);		 
				break;
			case COMMAND_CONFIGURE_FE_3:
				if (readUSARTAndUpdateFrequency(App::config.frequencySensor_C)) 
					Monitor::sendCommandResponse(COMMAND_CONFIGURE_FE_3, COMMAND_RESPONSE_SUCCESS);
				else
					Monitor::sendCommandResponse(COMMAND_CONFIGURE_FE_3, COMMAND_RESPONSE_FAILURE);		 
				break;
			case COMMAND_GET_CONFIG:
				Monitor::sendConfig(App::config);
				break;
		}
	}
}