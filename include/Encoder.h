#ifndef _ENCODER_H_
#define _ENCODER_H_
#include <Arduino.h>
#include <stdint.h>

class Encoder
{
public:
    Encoder(uint8_t pinA, uint8_t pinB);
    void begin();
    double SpeedPps();
    double SpeedRps();
    uint64_t AbsDeltaT_Micors(uint64_t t1, uint64_t t2);

private:
    uint32_t pulsecounter;
    uint64_t lasttime;
    uint8_t PinA, PinB;
    void IRQ_A();
    void IRQ_B();
    void MAttachInterruptPin(uint8_t pin, bool type);
    static Encoder *IA, *IB, *IC, *ID, *IE, *IF, *IG, *IH;
    static bool IA_, IB_, IC_, ID_, IE_, IF_, IG_, IH_;
    static void InterruptA();
    static void InterruptB();
    static void InterruptC();
    static void InterruptD();
    static void InterruptE();
    static void InterruptF();
    static void InterruptG();
    static void InterruptH();
};
#endif