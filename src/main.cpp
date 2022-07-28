#include <Arduino.h>
#include "LidarLd06.h"
#include "Vectors.hpp"
#include "F5WayLineTrackingModule.h"
#include "UltraSonic.h"
#include "UpperStateMachine.h"
#include "Encoder.h"
#include "Motor.h"

//#define AUTO_TUNE_PID

LidarLd06 lidar;
Motor motorL(MotorConfig{39, 38, 37, 2, 3, 1, 0.1, 0.1, true});
Motor motorR(MotorConfig{34, 35, 36, 4, 5, 1, 0.1, 0.1, true});
PinConfig5Way linetrackingCfg = {
    .Left = 26,
    .LMiddle = 27,
    .Middle = 28,
    .RMiddle = 29,
    .Right = 30,
    .TriggerLevel = HIGH};
UpperStateMachine statemachine(linetrackingCfg);

#ifndef AUTO_TUNE_PID
void setup()
{
  Serial.begin(230400);
  // lidar.begin(Serial1, 230400);
  // motorL.Rotate(3);
  // motorR.Rotate(3);
}

StateCommand last;

void loop()
{
  StateCommand cmd = statemachine.tracker.LineTrackingScan(linetrackingCfg, statemachine.goloop);
  if (last != cmd)
    switch (cmd)
    {
    case Left:
      motorL.RunPwm(60);
      motorR.RunPwm(90);
      break;
    case Right:
      motorL.RunPwm(90);
      motorR.RunPwm(60);
      break;
    case RapidLeft:
      motorL.RunPwm(10);
      motorR.RunPwm(90);
      break;
    case RapidRight:
      motorL.RunPwm(90);
      motorR.RunPwm(10);
      break;
    case Straight:
    default:
      motorL.RunPwm(90);
      motorR.RunPwm(90);
      break;
    }
  last = cmd;
  // motorL.update();
  // motorR.update();
}
#endif

#ifdef AUTO_TUNE_PID
#include <pidautotune.h>
void setup()
{

  PIDAutotuner tuner = PIDAutotuner();

  // Set the target value to tune to
  // This will depend on what you are tuning. This should be set to a value within
  // the usual range of the setpoint. For low-inertia systems, values at the lower
  // end of this range usually give better results. For anything else, start with a
  // value at the middle of the range.
  tuner.setTargetInputValue(4);

  // Set the loop interval in microseconds
  // This must be the same as the interval the PID control loop will run at
  tuner.setLoopInterval(5000);

  // Set the output range
  // These are the minimum and maximum possible output values of whatever you are
  // using to control the system (Arduino analogWrite, for example, is 0-255)
  tuner.setOutputRange(0, 255);

  // Set the Ziegler-Nichols tuning mode
  // Set it to either PIDAutotuner::ZNModeBasicPID, PIDAutotuner::ZNModeLessOvershoot,
  // or PIDAutotuner::ZNModeNoOvershoot. Defaults to ZNModeNoOvershoot as it is the
  // safest option.
  tuner.setZNMode(PIDAutotuner::ZNModeNoOvershoot);

  // This must be called immediately before the tuning loop
  // Must be called with the current time in microseconds
  tuner.startTuningLoop(micros());

  // Run a loop until tuner.isFinished() returns true
  long microseconds;
  while (!tuner.isFinished())
  {

    // This loop must run at the same speed as the PID control loop being tuned
    long prevMicroseconds = microseconds;
    microseconds = micros();

    // Get input value here (temperature, encoder position, velocity, etc)
    double input = motorL.SpeedRps();

    // Call tunePID() with the input value and current time in microseconds
    double output = tuner.tunePID(input, microseconds);

    // Set the output - tunePid() will return values within the range configured
    // by setOutputRange(). Don't change the value or the tuning results will be
    // incorrect.
    motorL.RunPwm(output);

    // This loop must run at the same speed as the PID control loop being tuned
    while (micros() - microseconds < 100)
      delayMicroseconds(1);
  }

  // Turn the output off here.
  motorL.RunPwm(0);

  // Get PID gains - set your PID controller's gains to these
  double kp = tuner.getKp();
  double ki = tuner.getKi();
  double kd = tuner.getKd();
  motorL.RunPwm(0);
}

void loop()
{

  // ...
}
#endif // AUTO_TUNE_PID