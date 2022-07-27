#include "5WayLineTrackingModule.h"
StateCommand CurrentState = NoState;

StateCommand LineTrackingScan(PinConfig5Way pinConfig, Loop loop)
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

    // 基本巡线逻辑 —— 单传感器压线
    if (left == pinConfig.TriggerLevel &&
        lmiddle != pinConfig.TriggerLevel &&
        middle != pinConfig.TriggerLevel &&
        rmiddle != pinConfig.TriggerLevel &&
        right != pinConfig.TriggerLevel) // ■□□□□
    {
        CurrentState = RapidLeft;
    }
    else if (left != pinConfig.TriggerLevel &&
             lmiddle != pinConfig.TriggerLevel &&
             middle != pinConfig.TriggerLevel &&
             rmiddle != pinConfig.TriggerLevel &&
             right == pinConfig.TriggerLevel) // □□□□■
    {
        CurrentState = RapidRight;
    }
    else if (left != pinConfig.TriggerLevel &&
             lmiddle == pinConfig.TriggerLevel &&
             rmiddle != pinConfig.TriggerLevel &&
             right != pinConfig.TriggerLevel &&
             CurrentState != RapidLeft &&
             CurrentState != RapidRight) // □■X□□
    {
        CurrentState = Left;
    }
    else if (left != pinConfig.TriggerLevel &&
             lmiddle != pinConfig.TriggerLevel &&
             rmiddle == pinConfig.TriggerLevel &&
             right != pinConfig.TriggerLevel &&
             CurrentState != RapidLeft &&
             CurrentState != RapidRight) // □□X■□
    {
        CurrentState = Right;
    }
    else if (left != pinConfig.TriggerLevel &&
             lmiddle != pinConfig.TriggerLevel &&
             middle == pinConfig.TriggerLevel &&
             rmiddle != pinConfig.TriggerLevel &&
             right != pinConfig.TriggerLevel) // □□■□□
    {
        CurrentState = Straight;
    }

    // 特殊情况 —— 多传感器压线或无传感器压线
    else if (left == pinConfig.TriggerLevel &&
             lmiddle == pinConfig.TriggerLevel &&
             middle == pinConfig.TriggerLevel &&
             rmiddle == pinConfig.TriggerLevel &&
             right == pinConfig.TriggerLevel) // ■■■■■
    {
        CurrentState = CheckPoint;
    }
    else if (left != pinConfig.TriggerLevel &&
             lmiddle != pinConfig.TriggerLevel &&
             middle != pinConfig.TriggerLevel &&
             rmiddle != pinConfig.TriggerLevel &&
             right != pinConfig.TriggerLevel &&
             CurrentState != RapidLeft &&
             CurrentState != RapidRight) // □□□□□
    {
        CurrentState = Pause_Straight_Loss;
    }
    else if (left == pinConfig.TriggerLevel &&
             (lmiddle == pinConfig.TriggerLevel ||
              middle == pinConfig.TriggerLevel ||
              rmiddle == pinConfig.TriggerLevel) &&
             right != pinConfig.TriggerLevel) // ■+++□  岔路
    {
        if (loop == Inner)
        {
            CurrentState = RapidLeft;
        }
        else
        {
            CurrentState = Straight;
        }
    }
    else if (left != pinConfig.TriggerLevel &&
             (lmiddle == pinConfig.TriggerLevel ||
              middle == pinConfig.TriggerLevel ||
              rmiddle == pinConfig.TriggerLevel) &&
             right == pinConfig.TriggerLevel) // □+++■  岔路
    {
        if (loop == Inner)
        {
            CurrentState = RapidRight;
        }
        else
        {
            CurrentState = Straight;
        }
    }
}