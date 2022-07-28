#include <Arduino.h>
#include "Motor.h"
#include "Encoder.h"
#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
#include <math.h>

Motor::Motor(MotorConfig config)
{
    cfg = config;
    encoder = new Encoder(cfg.EncoderPinA, cfg.EncoderPinB);
    pinMode(cfg.PhaseA1, OUTPUT);
    pinMode(cfg.PhaseA2, OUTPUT);
    pinMode(cfg.PWM, OUTPUT);
    digitalWrite(cfg.PhaseA1, LOW);
    digitalWrite(cfg.PhaseA2, LOW);
    pid = new PID(&speed, &pwmvalue, &target, cfg.kp, cfg.ki, cfg.kd, DIRECT);
    pid->SetMode(AUTOMATIC);
    pid->SetTunings(cfg.kp, cfg.ki, cfg.kd);
    pid->SetOutputLimits(0, 255);
}

Motor::~Motor()
{
    free(encoder);
}

double Motor::SpeedRps()
{
    return encoder->SpeedPps() / MOTOR_PPR;
}

void Motor::update()
{
    speed = SpeedRps();
    pid->Compute();
    RunPwm(pwmvalue);
}

void Motor::RunPwm(int pwm)
{
    // if (target == 0)
    //{
    //     digitalWrite(cfg.PhaseA1, LOW);
    //     digitalWrite(cfg.PhaseA2, LOW);
    //     return;
    // }
    analogWrite(cfg.PWM, pwm);
    if (pwm > 0)
    {
        if (!cfg.Inverted)
        {
            digitalWrite(cfg.PhaseA1, HIGH);
            digitalWrite(cfg.PhaseA2, LOW);
        }
        else
        {
            digitalWrite(cfg.PhaseA1, LOW);
            digitalWrite(cfg.PhaseA2, HIGH);
        }
    }
    else if (pwm < 0)
    {
        if (cfg.Inverted)
        {
            digitalWrite(cfg.PhaseA1, HIGH);
            digitalWrite(cfg.PhaseA2, LOW);
        }
        else
        {
            digitalWrite(cfg.PhaseA1, LOW);
            digitalWrite(cfg.PhaseA2, HIGH);
        }
    }
    else
    {
        digitalWrite(cfg.PhaseA1, LOW);
        digitalWrite(cfg.PhaseA2, LOW);
    }
}

void Motor::Rotate(double speed)
{
    target = speed;
}