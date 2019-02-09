#include "Monitor.hpp"
#include "USART.hpp"

namespace Communication {
    void Monitor::sendCommandResponse(uint8_t commandType, uint8_t responseCode){
        printf("%c%c", commandType, responseCode);
    }

    void Monitor::sendCommandData(uint8_t commandType, uint32_t data){
        
    }

    void Monitor::sendConfig(Application::Config config){
        printf("%c%c%c%c%c", COMMAND_GET_CONFIG, config.frequencySensor_A, config.frequencySensor_B, config.frequencySensor_C, config.delay_Mode_2);
        //TODO send current mode
    }

    ISR(USART_RX_vect){
        // Check for error
        if((UCSR0A & ((1 << FE0) | (1 << DOR0) | (1 << UPE0))) == 0) // Check frame error / data over run / parity error
        {
            Monitor::processReceivedMonitorChar(UDR0);
        }
    }

    bool Monitor::isWaitingForParameterChar = false;
    uint8_t Monitor::waitingCommand;

    void Monitor::processReceivedMonitorChar(uint8_t input){
        if (isWaitingForParameterChar){ // Process parameters if parameters are required
            isWaitingForParameterChar = false;
            processCommand(waitingCommand, input);
            return;
        }

        switch(input){ // Process commands and required parameters
            case COMMAND_CONFIGURE_FE_1:
            case COMMAND_CONFIGURE_FE_2:
            case COMMAND_CONFIGURE_FE_3:
            case COMMAND_CONFIGURE_MODE_2:
                if (!isWaitingForParameterChar) {
                    isWaitingForParameterChar = true;
                    waitingCommand = input;
                }
                break;
            default:
                cli();
                processCommand(input);
                sei();
                break;
        }
    }

    void Monitor::processCommand(uint8_t commandType, uint8_t parameter){
        switch(commandType){
			case COMMAND_STOP_MODE:
                #ifdef DEBUG
                USART::debug("stoping all");
                #endif // DEBUG

				Application::App::config.mode = APP_MODE_STOP;
				Application::App::saveConfig();
				Monitor::sendCommandResponse(COMMAND_STOP_MODE, COMMAND_RESPONSE_SUCCESS);
				break;
			case COMMAND_START_MODE_1:
                #ifdef DEBUG
                USART::debug("starting mode 1");
                #endif // DEBUG

				Application::App::config.mode = APP_MODE_1;
				Application::App::saveConfig();
				Monitor::sendCommandResponse(COMMAND_START_MODE_1, COMMAND_RESPONSE_SUCCESS);
				break;
			case COMMAND_START_MODE_2:
                #ifdef DEBUG
                USART::debug("starting mode 2");
                #endif // DEBUG

				Application::App::config.mode = APP_MODE_2;
				Application::App::saveConfig();
				Monitor::sendCommandResponse(COMMAND_START_MODE_2, COMMAND_RESPONSE_SUCCESS);
				break;
			case COMMAND_START_MODE_3:
                #ifdef DEBUG
                USART::debug("starting mode 3");
                #endif // DEBUG

				Application::App::config.mode = APP_MODE_3;
				Application::App::saveConfig();
				Monitor::sendCommandResponse(COMMAND_START_MODE_3, COMMAND_RESPONSE_SUCCESS);
				break;
			case COMMAND_CONFIGURE_FE_1:
                if (parameter >= 1 && parameter <= 20) {
                    #ifdef DEBUG
                    USART::debug("configure frequency 1");
                    #endif // DEBUG

                    Application::App::config.frequencySensor_A = parameter;
                    Application::App::saveConfig();
                    sendCommandResponse(COMMAND_CONFIGURE_FE_1, COMMAND_RESPONSE_SUCCESS);
                }
                else
                    sendCommandResponse(COMMAND_CONFIGURE_FE_1, COMMAND_RESPONSE_FAILURE);		 
				break;
			case COMMAND_CONFIGURE_FE_2:
				if (parameter >= 1 && parameter <= 20) {
                    #ifdef DEBUG
                    USART::debug("configure frequency 2");
                    #endif // DEBUG

                    Application::App::config.frequencySensor_B = parameter;
                    Application::App::saveConfig();
                    sendCommandResponse(COMMAND_CONFIGURE_FE_2, COMMAND_RESPONSE_SUCCESS);
                }
                else
                    sendCommandResponse(COMMAND_CONFIGURE_FE_2, COMMAND_RESPONSE_FAILURE);		 
				break;
			case COMMAND_CONFIGURE_FE_3:
				if (parameter >= 1 && parameter <= 20) {
                    #ifdef DEBUG
                    USART::debug("configure frequency 3");
                    #endif // DEBUG

                    Application::App::config.frequencySensor_C = parameter;
                    Application::App::saveConfig();
                    sendCommandResponse(COMMAND_CONFIGURE_FE_3, COMMAND_RESPONSE_SUCCESS);
                }
                else
                    sendCommandResponse(COMMAND_CONFIGURE_FE_3, COMMAND_RESPONSE_FAILURE);	
				break;
            case COMMAND_CONFIGURE_MODE_2:
				if (parameter >= 1 && parameter <= 5) {
                    #ifdef DEBUG
                    USART::debug("configure timer");
                    #endif // DEBUG

                    Application::App::config.delay_Mode_2 = parameter;
                    Application::App::saveConfig();
                    sendCommandResponse(COMMAND_CONFIGURE_MODE_2, COMMAND_RESPONSE_SUCCESS);
                }
                else
                    sendCommandResponse(COMMAND_CONFIGURE_MODE_2, COMMAND_RESPONSE_FAILURE);	
				break;
            case COMMAND_GET_DATA:
                #ifdef DEBUG
                USART::debug("sendData");
                #endif // DEBUG

				Application::App::sendAllDataFlag = true;
				break;
			case COMMAND_GET_CONFIG:
                #ifdef DEBUG
                USART::debug("sendingConfig");
                #endif // DEBUG

				Monitor::sendConfig(Application::App::config);
				break;
		}
    }
}