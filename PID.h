/* 
 * File:   PID.h
 * Author: matt
 *
 * Created on 17 February 2013, 11:19
 */

#ifndef PID_H
#define	PID_H

class PIDClass
{
public:
    PIDClass();
    PIDClass(const PIDClass& orig);
    virtual ~PIDClass();
    void initialise(float KP, float KI, float KD, float ILIM, float LIM, int DFILLEN);
    void calculate(double* position, float* setpoint, float* dt);
    void calculate(double* position, double* setpoint, float* dt);
    void setPID(float KP, float KI, float KD);
    void getPID();
    float output;
private:
    void constrain_(float* value, float range);
    float proportional, integral, derivative;
    float dHist[32];
    float kp;
    float ki;
    float kd;
    float ilim;
    float lim;
    int dFilLen;
    float error;
    float prevError;    
};

#endif	/* PID_H */

