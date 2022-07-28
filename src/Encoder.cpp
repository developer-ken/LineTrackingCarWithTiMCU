#include <Arduino.h>
#include "Encoder.h"

Encoder *Encoder::IA = nullptr, *Encoder::IB = nullptr, *Encoder::IC = nullptr, *Encoder::ID = nullptr,
        *Encoder::IE = nullptr, *Encoder::IF = nullptr, *Encoder::IG = nullptr, *Encoder::IH = nullptr;
bool Encoder::IA_, Encoder::IB_, Encoder::IC_, Encoder::ID_, Encoder::IE_, Encoder::IF_, Encoder::IG_, Encoder::IH_;

Encoder::Encoder(uint8_t pinA, uint8_t pinB)
{
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
    MAttachInterruptPin(pinA, true);
    MAttachInterruptPin(pinB, false);
    PinA = pinA;
    PinB = pinB;
    pulsecounter = 0;
}

double Encoder::SpeedPps()
{
    int16_t delta = AbsDeltaT_Micors(lasttime, micros());
    lasttime = micros();
    double pps = (pulsecounter * 1.0 / delta) * 1000000;
    pulsecounter = 0;
    return pps;
}

void Encoder::IRQ_A()
{
    pulsecounter++;
}

void Encoder::IRQ_B()
{
    pulsecounter++;
}

uint64_t Encoder::AbsDeltaT_Micors(uint64_t t1, uint64_t t2)
{
    if (t1 < t2)
        return t2 - t1;
    else
        return UINT64_MAX - (t1 - t2);
}

void Encoder::MAttachInterruptPin(uint8_t pin, bool type)
{
    int attached = 0;
    if (IA == nullptr)
    {
        attachInterrupt(pin, InterruptA, CHANGE);
        IA = this;
        IA_ = type;
        attached++;
    }
    else if (attached < 2 && IB == nullptr)
    {
        attachInterrupt(pin, InterruptB, CHANGE);
        IB = this;
        IB_ = type;
        attached++;
    }
    else if (attached < 2 && IC == nullptr)
    {
        attachInterrupt(pin, InterruptC, CHANGE);
        IC = this;
        IC_ = type;
        attached++;
    }
    else if (attached < 2 && ID == nullptr)
    {
        attachInterrupt(pin, InterruptD, CHANGE);
        ID = this;
        ID_ = type;
        attached++;
    }
    else if (attached < 2 && IE == nullptr)
    {
        attachInterrupt(pin, InterruptE, CHANGE);
        IE = this;
        IE_ = type;
        attached++;
    }
    else if (attached < 2 && IF == nullptr)
    {
        attachInterrupt(pin, InterruptF, CHANGE);
        IF = this;
        IF_ = type;
        attached++;
    }
    else if (attached < 2 && IG == nullptr)
    {
        attachInterrupt(pin, InterruptG, CHANGE);
        IG = this;
        IG_ = type;
        attached++;
    }
    else if (attached < 2 && IH == nullptr)
    {
        attachInterrupt(pin, InterruptH, CHANGE);
        IH = this;
        IH_ = type;
        attached++;
    }
}
void Encoder::InterruptA()
{
    if (IA != nullptr)
        if (IA_)
        {
            IA->IRQ_A();
        }
        else
            IA->IRQ_B();
}
void Encoder::InterruptB()
{
    if (IB != nullptr)
        if (IB_)
        {
            IB->IRQ_A();
        }
        else
            IB->IRQ_B();
}
void Encoder::InterruptC()
{
    if (IC != nullptr)
        if (IC_)
        {
            IC->IRQ_A();
        }
        else
            IC->IRQ_B();
}
void Encoder::InterruptD()
{
    if (ID != nullptr)
        if (ID_)
        {
            ID->IRQ_A();
        }
        else
            ID->IRQ_B();
}
void Encoder::InterruptE()
{
    if (IE != nullptr)
        if (IE_)
        {
            IE->IRQ_A();
        }
        else
            IE->IRQ_B();
}
void Encoder::InterruptF()
{
    if (IF != nullptr)
        if (IF_)
        {
            IF->IRQ_A();
        }
        else
            IF->IRQ_B();
}
void Encoder::InterruptG()
{
    if (IG != nullptr)
        if (IG_)
        {
            IG->IRQ_A();
        }
        else
            IG->IRQ_B();
}
void Encoder::InterruptH()
{
    if (IH != nullptr)
        if (IH_)
        {
            IH->IRQ_A();
        }
        else
            IH->IRQ_B();
}