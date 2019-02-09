#ifndef MONITOR_H
#define MONITOR_H

#include <Arduino.h>
#include "Config.hpp"
#include "App.hpp"

#ifdef DEBUG_COMMANDS
#define COMMAND_STOP_MODE 97 //a
#define COMMAND_START_MODE_1 98 //b
#define COMMAND_START_MODE_2 99 //c
#define COMMAND_START_MODE_3 100 //d
#define COMMAND_GET_DATA 101 //e
#define COMMAND_CONFIGURE_FE_1 102 //f
#define COMMAND_CONFIGURE_FE_2 103 //g
#define COMMAND_CONFIGURE_FE_3 104 //h
#define COMMAND_CONFIGURE_MODE_2 105//i
#define COMMAND_SEND_MODE1_DATA 106 //j
#define COMMAND_SEND_MODE2_DATA 107 //k
#define COMMAND_GET_CONFIG 108 //l
#define COMMAND_DEBUG 109 //m

#else
#define COMMAND_STOP_MODE 0x0
#define COMMAND_START_MODE_1 0x1
#define COMMAND_START_MODE_2 0x2
#define COMMAND_START_MODE_3 0x3
#define COMMAND_GET_DATA 0x4
#define COMMAND_CONFIGURE_FE_1 0x5
#define COMMAND_CONFIGURE_FE_2 0x6
#define COMMAND_CONFIGURE_FE_3 0x7
#define COMMAND_CONFIGURE_MODE_2 0x8
#define COMMAND_SEND_MODE1_DATA 0x9
#define COMMAND_SEND_MODE2_DATA 0xA
#define COMMAND_GET_CONFIG 0xB
#define COMMAND_DEBUG 0xF
#endif

#define COMMAND_RESPONSE_SUCCESS 0x0
#define COMMAND_RESPONSE_FAILURE 0x1

namespace Communication {
    class Monitor {
    public:
        static void sendCommandResponse(uint8_t commandType, uint8_t responseCode);
        static void sendCommandData(uint8_t commandType, uint32_t data);
        static void sendConfig(Application::Config config);
        inline static void processReceivedMonitorChar(uint8_t input);
        inline static void processCommand(uint8_t commandType, uint8_t parameter = 0);
    private:
        static bool isWaitingForParameterChar;
        static uint8_t waitingCommand;
    };
}

#endif //Monitor.hpp