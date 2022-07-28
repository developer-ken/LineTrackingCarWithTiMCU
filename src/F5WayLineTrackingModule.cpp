#include "F5WayLineTrackingModule.h"
StateCommand LineTraker::LineTrackingScan(PinConfig5Way pinConfig, Loop loop)
{
    pinMode(pinConfig.Left, INPUT);
    pinMode(pinConfig.LMiddle, INPUT);
    pinMode(pinConfig.Middle, INPUT);
    pinMode(pinConfig.RMiddle, INPUT);
    pinMode(pinConfig.Right, INPUT);

    uint8_t left = digitalRead(pinConfig.Left);
    uint8_t lmiddle = digitalRead(pinConfig.LMiddle);
    uint8_t middle = digitalRead(pinConfig.Middle);
    uint8_t rmiddle = digitalRead(pinConfig.RMiddle);
    uint8_t right = digitalRead(pinConfig.Right);

    uint8_t contacts = left + lmiddle + middle + rmiddle + right;
    if (pinConfig.TriggerLevel == LOW)
    {
        contacts = 5 - contacts;
    }

    if (contacts > 3)
    {
        CurrentState = CheckPoint;

    }else if(contacts>1){
        CurrentState = 
    }

    if (left == pinConfig.TriggerLevel)
    {
        CurrentState = RapidLeft;
    }
    else if (right == pinConfig.TriggerLevel)
    {
        CurrentState = RapidRight;
    }
    if (lmiddle == pinConfig.TriggerLevel)
    {
        CurrentState = Left;
    }
    else if (rmiddle == pinConfig.TriggerLevel)
    {
        CurrentState = Right;
    }
    else if (middle == pinConfig.TriggerLevel)
    {
        if (CurrentState == Left || CurrentState == Right)
            CurrentState = Straight;
    }
    return CurrentState;
}