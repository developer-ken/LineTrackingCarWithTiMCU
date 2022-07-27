#include <Arduino.h>

#include "LidarLd06.h"
#include "Vectors.hpp"
#include "5WayLineTrackingModule.h"
#include "UltraSonic.h"

LidarLd06 lidar;
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  lidar.begin(Serial1, 230400);
}

void loop()
{
  // put your main code here, to run repeatedly:
}