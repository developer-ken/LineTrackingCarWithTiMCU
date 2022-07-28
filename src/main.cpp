#include <Arduino.h>

#include "LidarLd06.h"
#include "Vectors.hpp"
#include "F5WayLineTrackingModule.h"
#include "UltraSonic.h"
#include "UpperStateMachine.h"

LidarLd06 lidar;
PinConfig5Way linetrackingCfg = {
    .Left = 0,
    .LMiddle = 1,
    .Middle = 2,
    .RMiddle = 3,
    .Right = 4,
    .TriggerLevel = HIGH};
UpperStateMachine statemachine(linetrackingCfg);

void setup()
{
  Serial.begin(115200);
  lidar.begin(Serial1, 230400);
}

void loop()
{
  StateCmd cmd = statemachine.update();
}