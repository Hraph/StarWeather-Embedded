#include "Monitor.hpp"

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
}