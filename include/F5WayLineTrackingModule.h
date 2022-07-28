#ifndef _5WAYLINETRACKINGMODULE_H
#define _5WAYLINETRACKINGMODULE_H
#include <Arduino.h>
#include <stdint.h>

struct
{
    uint8_t Left, LMiddle, Middle, RMiddle, Right; // Pin numbers
    uint8_t TriggerLevel;                          // HIGH or LOW
} typedef PinConfig5Way;

enum
{
    Inner = 0,
    Outter = 1
} typedef Loop;

enum StateCommand
{
    RapidLeft = -2,
    Left = -1,
    Straight = 0,
    Right = 1,
    RapidRight = 2,

    CheckPoint = 3,
    Pause = 4,
    Loss = 5,
    NoState = 10
} typedef StateCommand;

class LineTraker
{
public:
    bool IsAtCrossing;
    StateCommand CurrentState;
    StateCommand LineTrackingScan(PinConfig5Way, Loop);

private:
    uint16_t countdown;
};
#endif