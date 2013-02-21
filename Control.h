/* 
 * File:   Control.h
 * Author: matt
 *
 * Created on 08 November 2012, 16:05
 */

#ifndef CONTROL_H
#define	CONTROL_H

#include "PID.h"

struct s_orientation
{
    double phi;
    double psi;
    double theta;
};

struct s_PIDConstants
{
    float kp;
    float ki;
    float kd;
};

struct s_PID
{
    s_orientation differential;
    s_orientation integral;
    s_PIDConstants constants;
    s_PIDConstants yawConstants;
    s_orientation error;
    s_orientation prevError;
    s_orientation output;
};

struct s_altitudePID
{
    double differential;
    double integral;
    double error;
    double prevError;
    double output;
    s_PIDConstants constants;
};

class ControlClass
{
public:
    ControlClass();
    ControlClass(const ControlClass& orig);
    virtual ~ControlClass();

    void update();
    void setRatePID(float KP, float KI, float KD);
    void getRatePID();
    void setAttitudePID(float KP, float KI, float KD);
    void getAttitudePID();
    s_altitudePID altitudePID;
    PIDClass ratePitchPID, rateRollPID, rateYawPID;
    PIDClass attitudePitchPID, attitudeRollPID;

private:
    void updatePWM_(float* throttle, float* pitch, float* roll, float* yaw);
    void evaluateAltitudeControl_();
    void altitudeControl_();
    void rateControl_(float* pitchrate, float* rollrate, float* yawrate);
    void attitudeControl_(float* targetPitch, float* targetRoll, float* targetYaw);
    void constrain_(double* value, float range);
#define differentialFilterSize 5
    s_orientation differentialBuf[differentialFilterSize];
    double targetAltitude_;
    bool altitudeHoldActive_;
};

extern ControlClass Control;

#endif	/* CONTROL_H */

