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
        IsAtCrossing = false;
    }
    else if (contacts > 1)
    {
        if (loop == Inner)
            CurrentState = RapidLeft;
        else
            CurrentState = Right;
        IsAtCrossing = true;
    }
    else if (contacts == 0)
    {
        if (CurrentState == CheckPoint)
            CurrentState = Pause;
    }
    else
    {
        if (left == pinConfig.TriggerLevel)
        {
            CurrentState = RapidLeft;
            IsAtCrossing = false;
        }
        else if (right == pinConfig.TriggerLevel)
        {
            CurrentState = RapidRight;
            IsAtCrossing = false;
        }
        if (lmiddle == pinConfig.TriggerLevel)
        {
            CurrentState = Left;
            IsAtCrossing = false;
        }
        else if (rmiddle == pinConfig.TriggerLevel)
        {
            CurrentState = Right;
            IsAtCrossing = false;
        }
        else if (middle == pinConfig.TriggerLevel)
        {
            if (CurrentState == Left || CurrentState == Right)
                CurrentState = Straight;
            IsAtCrossing = false;
        }
    }
    return CurrentState;
}