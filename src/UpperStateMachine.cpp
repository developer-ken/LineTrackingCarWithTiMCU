#include "UpperStateMachine.h"

UpperStateMachine::UpperStateMachine(PinConfig5Way Config)
{
    this->Config = Config;
    state = Loss;
    goloop = Inner;
    AtCrossing = false;
    CountCrossing = 0;
    CountPausePoint = 0;
    CountStopPoint = 0;
}

StateCmd UpperStateMachine::update()
{
    bool crossing = false;
    StateCommand statec = LineTrackingScan(Config, goloop, &crossing);
    if (LastState == CheckPoint && statec != CheckPoint)
    {
        if (statec == Pause_Straight_Loss)
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
        if (statec == Pause_Straight_Loss)
            state = Loss;
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
}