/* 
 * File:   PID.cpp
 * Author: matt
 * 
 * Created on 17 February 2013, 11:19
 */

#include "PID.h"
#include <iostream>

PIDClass::PIDClass() {
    dFilK = 0;
}

PIDClass::PIDClass(const PIDClass& orig) {
}

PIDClass::~PIDClass() {
}

void PIDClass::initialise(float KP, float KI, float KD, float ILIM, float LIM, int DFILLEN, double* ALT_DERIV_SOURCE) {
    kp = KP;
    ki = KI;
    kd = KD;
    ilim = ILIM;
    lim = LIM;
    dFilLen = DFILLEN;
    altDerivativeSource = ALT_DERIV_SOURCE;
}

void PIDClass::setPID(float KP, float KI, float KD) {
    kp = KP;
    ki = KI;
    kd = KD;
}

void PIDClass::getPID() {
    std::cout << kp << ", " << ki << ", " << kd << std::endl;
}

void PIDClass::calculate(double* position, float* setpoint, float* dt) {
    prevError = error;
    error = *setpoint - *position;
    integral += error * *dt;

    if(altDerivativeSource == NULL) {
	//Derivative low pass filter
	//Store current derivative value into history table
	dHist[dFilK] = (error - prevError) / *dt;
	dFilK++;
	if(dFilK == dFilLen) {
	    dFilK = 0;
	}
	//Average history table
	derivative = 0;
	for(int k = 0; k < dFilLen; k++) {
	    derivative += dHist[k];
	}
	derivative /= dFilLen;
    }
    else{
	derivative = -*altDerivativeSource;
    }

    //Anti-windup
    constrain_(&integral, ilim);

    output = error * kp + integral * ki + derivative*kd;

    //Anti-saturation
    constrain_(&output, lim);
}

//void PIDClass::calculate(double* position, float* setpoint, float* dt) {
//    prevError = error;
//    error = *setpoint - *position;
//    proportional = error;
//    integral += error * *dt;
//    derivative = (error - prevError) / *dt;
//
//    //Anti-windup
//    constrain_(&integral, ilim);
//
//    output = proportional * kp + integral * ki + derivative*kd;
//
//    //Anti-saturation
//    constrain_(&output, lim);
//}

inline void PIDClass::constrain_(float* value, float range) {
    if(*value > range) *value = range;
    else if(*value < -range) *value = -range;
}
