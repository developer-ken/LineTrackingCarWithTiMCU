#include <Arduino.h>
#include <arduino-timer.h>
#include "LidarLd06.h"
#include "Vectors.hpp"
#include "F5WayLineTrackingModule.h"
#include "UltraSonic.h"
#include "UpperStateMachine.h"
#include "Encoder.h"
#include "Motor.h"
#include "Logger.h"

#define MAX_BASE_SPEED 90

//#define AUTO_TUNE_PID

LidarLd06 lidar;
Timer<10, millis> timer;
Motor motorL(MotorConfig{39, 38, 37, 6, 7, 1, 0.1, 0.1, true});
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

bool update(void *argument);
bool update_lidar(void *argument);
double closest_distance = INFINITY; // Deg = 200~320

void setup()
{
  Serial.begin(115200);
  Serial1.begin(230400);
  Serial.println("20220729 by dev_ken | booting...");
  Logger::serial = &Serial;
  // pinMode(PB_2, OUTPUT);
  lidar.begin();
  // motorL.Rotate(3);
  // motorR.Rotate(3);
  timer.every(100, update);
  timer.every(5, update_lidar);
}

StateCommand last;
uint8_t loopnum = 0;
uint8_t basespeed = 70;

bool update(void *argument)
{
  if (lidar.ClosestPoint.Angle >= 200 && lidar.ClosestPoint.Angle <= 320)
  { //仅由前方触发  此判断是冗余的，在LidarLd06中也有相关过滤器
    closest_distance = lidar.ClosestPoint.Distance;
  }
  else
  {
    closest_distance = INFINITY;
  }
  Logger::Log(String(lidar.ClosestPoint.Angle) + "->" + String(lidar.ClosestPoint.Distance));
  if(closest_distance <185){
    basespeed = 0;
    last = NoState;
  }
  if (closest_distance < 200 && basespeed > 0)
  {
    basespeed--;
    last = NoState;
  }
  else if (closest_distance > 200 && basespeed < MAX_BASE_SPEED)
  {
    basespeed++;
    last = NoState;
  }


  return true;
}

bool update_lidar(void *argument)
{
  lidar.update();
  return true;
}

void loop()
{
  timer.tick();
  StateCommand cmd = statemachine.tracker.LineTrackingScan(linetrackingCfg, (Loop)(loopnum % 2));
  if (last != cmd)
    switch (cmd)
    {
    case Left:
      motorL.RunPwm(basespeed - 20);
      motorR.RunPwm(basespeed + 10);
      break;
    case Right:
      motorL.RunPwm(basespeed + 10);
      motorR.RunPwm(basespeed - 20);
      break;
    case RapidLeft:
      motorL.RunPwm(10);
      motorR.RunPwm(basespeed + 10);
      digitalWrite(PB_2, 0);
      break;
    case RapidRight:
      motorL.RunPwm(basespeed + 10);
      motorR.RunPwm(10);
      break;
    case Pause:
      motorL.RunPwm(0);
      motorR.RunPwm(0);
      delay(3000);
      motorL.RunPwm(basespeed + 10);
      motorR.RunPwm(basespeed + 10);
    case CheckPoint:
      loopnum++;
      analogWrite(PB_2, 255 / 2);
    case Straight:
    default:
      motorL.RunPwm(basespeed + 10);
      motorR.RunPwm(basespeed + 10);
      break;
    }
  last = cmd;
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