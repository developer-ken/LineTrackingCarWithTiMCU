#ifndef _UPPERSTATEMACHINE_H_
#define _UPPERSTATEMACHINE_H_
#include <Arduino.h>
#include "F5WayLineTrackingModule.hpp"

enum StateCmd
{
    SRapidLeft = -2,
    SLeft = -1,
    SStraight = 0,
    SRight = 1,
    SRapidRight = 2,

    PausePoint = 3,
    StopPoint = 4,
    Loss = 5
} typedef StateCmd;

class UpperStateMachine
{
public:
    PinConfig5Way Config;
    UpperStateMachine(PinConfig5Way);
    StateCmd state;
    StateCmd update();
    Loop goloop;
    uint8_t CountCrossing;
    uint8_t CountPausePoint;
    uint8_t CountStopPoint;

private:
    StateCommand LastState;
    int statetimer;
    bool AtCrossing;
    LineTraker tracker;
};
#endif