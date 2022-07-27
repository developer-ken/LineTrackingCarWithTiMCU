#include "UltraSonic.h"

double MeasureOnce(uint8_t pin){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delayMicroseconds(2);
    digitalWrite(pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin, LOW);
    pinMode(pin, INPUT);
    double duration = pulseIn(pin, HIGH);
    double distance = duration / 29 / 2;
    return distance;
}