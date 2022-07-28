#ifndef _MOTOR_H_
#define _MOTOR_H_
#include <stdint.h>
#include <PID_v1.h>
#include <Encoder.h>
#include <PID_AutoTune_v0.h>

#define MOTOR_PPR (1040 * 64)

struct MotorConfig
{
    uint8_t PhaseA1, PhaseA2, PWM;
    uint8_t EncoderPinA, EncoderPinB;
    double kp, ki, kd;
    bool Inverted;
} typedef MotorConfig;

struct Motors
{
    MotorConfig Left, Right;
};

class Motor
{
public:
    Motor(MotorConfig);
    ~Motor();
    void Rotate(double rps_speed);
    double SpeedRps();
    void update();
    void RunPwm(int pwm);
    MotorConfig cfg;
    Encoder *encoder;
    PID *pid;
    double speed, pwmvalue, target;

private:
    double lastpps;
    int32_t lastposition;
};
#endif