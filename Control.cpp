/* 
 * File:   Control.cpp
 * Author: matt
 * 
 * Created on 08 November 2012, 16:05
 */

#include "Control.h"
#include "PICInterface.h"
#include "AHRS.h"

const uint16_t MOTOR_MAX = 20000;
const uint16_t MOTOR_MIN = 9000;

ControlClass Control;

ControlClass::ControlClass()
{
    pidconstants.kp = 17;
    pidconstants.ki = 30;
    pidconstants.kd = 10;
    
    yawPIDConstants.kp = 40;
    yawPIDConstants.ki = 0;
    yawPIDConstants.kd = 0;
}

ControlClass::ControlClass(const ControlClass& orig)
{
}

ControlClass::~ControlClass()
{
}

void ControlClass::update()
{
    prevError = error;

    error.phi = PICInterface.rx.pitch - AHRS.orientation.phi;
    error.psi = PICInterface.rx.roll - AHRS.orientation.psi;
    error.theta = PICInterface.rx.yaw - AHRS.calibratedData.r; //Yaw uses rate control instead of angle

    differential.phi = (error.phi - prevError.phi) / Timer.dt;
    differential.psi = (error.psi - prevError.psi) / Timer.dt;
    differential.theta = (error.theta - prevError.theta) / Timer.dt;

    integral.phi += error.phi * Timer.dt;
    integral.psi += error.psi * Timer.dt;
    integral.theta += error.theta * Timer.dt;
    
    constrain_ (&integral.phi, 0.5);
    constrain_ (&integral.psi, 0.5);
    constrain_ (&integral.theta, 0.5);

    pid.phi = pidconstants.kp * error.phi + pidconstants.kd * differential.phi + pidconstants.ki * integral.phi;
    pid.psi = pidconstants.kp * error.psi + pidconstants.kd * differential.psi + pidconstants.ki * integral.psi;
    pid.theta = yawPIDConstants.kp * error.theta;// + yawPIDConstants.kd * differential.theta;


    PICInterface.pwmwidths.frontleft = ((PICInterface.rx.throttle * (MOTOR_MAX - MOTOR_MIN)) + MOTOR_MIN) - pid.phi - pid.psi + pid.theta;
    PICInterface.pwmwidths.frontright = ((PICInterface.rx.throttle * (MOTOR_MAX - MOTOR_MIN)) + MOTOR_MIN) + pid.phi - pid.psi - pid.theta;
    PICInterface.pwmwidths.rearright = ((PICInterface.rx.throttle * (MOTOR_MAX - MOTOR_MIN)) + MOTOR_MIN) + pid.phi + pid.psi + pid.theta;
    PICInterface.pwmwidths.rearleft = ((PICInterface.rx.throttle * (MOTOR_MAX - MOTOR_MIN)) + MOTOR_MIN) - pid.phi + pid.psi - pid.theta;
    PICInterface.setPWM();
}

inline void ControlClass::constrain_(double* value, float range)
{
    if (*value > range) *value = range;
    else if (*value < -range) *value = -range;
}

void ControlClass::setPID(int p, int i, int d)
{
    pidconstants.kp = p;
    pidconstants.ki = i;
    pidconstants.kd = d;
}

void ControlClass::getPID()
{
    std::cout << pidconstants.kp << ", " << pidconstants.ki << ", " << pidconstants.kd << std::endl;
}

void ControlClass::setYawPID(int p, int i, int d)
{
    yawPIDConstants.kp = p;
    yawPIDConstants.ki = i;
    yawPIDConstants.kd = d;
}

void ControlClass::getYawPID()
{
    std::cout << yawPIDConstants.kp << ", " << yawPIDConstants.ki << ", " << yawPIDConstants.kd << std::endl;
}