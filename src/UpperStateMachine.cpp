#include "UpperStateMachine.h"
#include "F5WayLineTrackingModule.h"

UpperStateMachine::UpperStateMachine(PinConfig5Way Config)
{
    this->Config = Config;
    state = SLoss;
    goloop = Inner;
    AtCrossing = false;
    CountCrossing = 0;
    CountPausePoint = 0;
    CountStopPoint = 0;
    tracker = LineTraker();
}

StateCmd UpperStateMachine::update()
{
    bool crossing = false;
    StateCommand statec = tracker.LineTrackingScan(Config, goloop);
    crossing = tracker.IsAtCrossing;
    if (LastState == CheckPoint && statec != CheckPoint)
    {
        if (statec == Loss)
        {
            state = PausePoint;
            CountPausePoint++;
        }
        else if (statec == Straight || statec == Left || statec == Right)
        {
            state = StopPoint;
            CountStopPoint++;
        }
    }
    else if (statec <= 2)
    {
        state = (StateCmd)statec;
    }
    else
    {
        if (statec == Loss)
            state = SLoss;
    }
    if (crossing && !AtCrossing)
    {
        AtCrossing = true;
        CountCrossing++;
    }
    else if (!crossing && AtCrossing)
    {
        AtCrossing = false;
    }
    LastState = statec;
    return state;
}