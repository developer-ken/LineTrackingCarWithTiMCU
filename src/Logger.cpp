#include <Arduino.h>
#include "Logger.h"
#include <HardwareSerial.h>

void Logger::Log(String str)
{
    if (serial != nullptr)
        serial->println(str);
}

HardwareSerial *Logger::serial = nullptr;