#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <Arduino.h>

class Logger
{
public:
    static HardwareSerial* serial;
    static void Log(String);
};
#endif