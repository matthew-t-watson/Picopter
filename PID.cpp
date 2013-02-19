/* 
 * File:   PID.cpp
 * Author: matt
 * 
 * Created on 17 February 2013, 11:19
 */

#include "PID.h"
#include <iostream>

PIDClass::PIDClass() {
}

PIDClass::PIDClass(const PIDClass& orig) {
}

PIDClass::~PIDClass() {
}

void PIDClass::initialise(float KP, float KI, float KD, float ILIM, float LIM){
    kp = KP;
    ki = KI;
    kd = KD;
    ilim = ILIM;
    lim = LIM;
}

void PIDClass::setPID(float KP, float KI, float KD){
    kp = KP;
    ki = KI;
    kd = KD;
}

void PIDClass::getPID(){
    std::cout << kp << ", " << ki << ", " << kd << std::endl;
}

void PIDClass::calculate(double* position, double* setpoint, float* dt){
    prevError = error;
    error = setpoint - position;
    p = error;
    i += error * *dt;
    d = (error - prevError) / *dt;
    
    //Anti-windup
    constrain_(&i, ilim);
    
    output = p*kp + i*ki + d*kd;
    
    //Anti-saturation
    constrain_(&output, lim);
}

void PIDClass::calculate(double* position, float* setpoint, float* dt){
    prevError = error;
    error = *setpoint - *position;
    p = error;
    i += error * *dt;
    d = (error - prevError) / *dt;
    
    //Anti-windup
    constrain_(&i, ilim);
    
    output = p*kp + i*ki + d*kd;
    
    //Anti-saturation
    constrain_(&output, lim);
}

inline void PIDClass::constrain_(float* value, float range) {
    if(*value > range) *value = range;
    else if(*value < -range) *value = -range;
}
