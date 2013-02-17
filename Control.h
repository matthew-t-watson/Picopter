/* 
 * File:   Control.h
 * Author: matt
 *
 * Created on 08 November 2012, 16:05
 */

#ifndef CONTROL_H
#define	CONTROL_H

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
    void setAttitudePID(int p, int i, int d);
    void getAttitudePID();
    void setYawPID(int p, int i, int d);
    void getYawPID();
    s_PID attitudePID;
    s_altitudePID altitudePID;
    
private:
    void updatePWM_();
    void attitudeControl_();
    void evaluateAltitudeControl_();
    void altitudeControl_();
    void constrain_(double* value, float range);
#define differentialFilterSize 5
    s_orientation differentialBuf[differentialFilterSize];
    double targetAltitude_;
    bool alreadySet_;
};

extern ControlClass Control;

#endif	/* CONTROL_H */

