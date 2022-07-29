#include "F5WayLineTrackingModule.h"
#include "Logger.h"

StateCommand LineTraker::LineTrackingScan(PinConfig5Way pinConfig, Loop loop)
{
    StLock = Normal;
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

    if (StLock < 0)
    {
        rmiddle = !pinConfig.TriggerLevel;
        middle = !pinConfig.TriggerLevel;
    }
    else if (StLock > 0)
    {
        lmiddle = !pinConfig.TriggerLevel;
        middle = !pinConfig.TriggerLevel;
    }
    if (right == pinConfig.TriggerLevel && StLock == EnterLeftStage1)
    {
        StLock = EnterLeftStage2;
        right = !pinConfig.TriggerLevel;
    }
    else if (right != pinConfig.TriggerLevel && StLock == EnterLeftStage2)
    {
        StLock = Normal;
    }

    if (left == pinConfig.TriggerLevel && StLock == EnterRightStage1)
    {
        StLock = EnterRightStage2;
        left = !pinConfig.TriggerLevel;
    }
    else if (left != pinConfig.TriggerLevel && StLock == EnterRightStage2)
    {
        StLock = Normal;
    }

    uint8_t contacts = left + lmiddle + middle + rmiddle + right;
    if (pinConfig.TriggerLevel == LOW)
    {
        contacts = 5 - contacts;
    }

    if (contacts > 2)
    {
        if (CurrentState != CheckPoint && CurrentState != NoState)
        { 
            CurrentState = CheckPoint;
            IsAtCrossing = false;
            Logger::Log("Cp");
        }
        else
        {
            CurrentState = NoState;
        }
    }
    else if (contacts > 1 && contacts <= 2)
    {
        if ((left == pinConfig.TriggerLevel && lmiddle != pinConfig.TriggerLevel) ||
            (lmiddle == pinConfig.TriggerLevel && left != pinConfig.TriggerLevel && middle != pinConfig.TriggerLevel) ||
            (middle == pinConfig.TriggerLevel && lmiddle != pinConfig.TriggerLevel && rmiddle != pinConfig.TriggerLevel) ||
            (rmiddle == pinConfig.TriggerLevel && middle != pinConfig.TriggerLevel && right != pinConfig.TriggerLevel) ||
            (right == pinConfig.TriggerLevel && rmiddle != pinConfig.TriggerLevel))
        {
            Logger::Log("Crossing..");
            if (loop == Inner)
            {
                CurrentState = RapidLeft;
                StLock = EnterLeftStage1;
                Logger::Log("Go in");
            }
            else
            {
                StLock = EnterRightStage1;
                CurrentState = RapidRight;
                Logger::Log("Avoid");
            }
            IsAtCrossing = true;
        }
    }
    else if (contacts == 0)
    {
        // Logger::Log("C=0...");
        if (CurrentState == CheckPoint)
        {
            Logger::Log("Cp,Pause");
            CurrentState = Pause;
        }
    }
    else
    {
        // Logger::Log("Nrm");
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