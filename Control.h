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

struct s_PID
{
    float kp;
    float ki;
    float kd;
};

class ControlClass
{
public:
    ControlClass();
    ControlClass(const ControlClass& orig);
    virtual ~ControlClass();
    
    void update();
    void setPID(int p, int i, int d);
    void getPID();
    void setYawPID(int p, int i, int d);
    void getYawPID();
    s_orientation integral;
private:
    void constrain_(double* value, float range);
    
    s_orientation error;
    s_orientation prevError;
    s_orientation differential;
    s_orientation pid;
    s_PID pidconstants;
    s_PID yawPIDConstants;
};

extern ControlClass Control;

#endif	/* CONTROL_H */

