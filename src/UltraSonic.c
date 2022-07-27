#include "UltraSonic.h"

double MeasureOnce(uint8_t pin)
{
    double duration;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delayMicroseconds(2);
    digitalWrite(pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin, LOW);
    pinMode(pin, INPUT);
    duration = pulseIn(pin, HIGH, 100);
    return duration ? duration / 29 / 2 : INFINITY;
}