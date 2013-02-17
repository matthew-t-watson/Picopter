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

ControlClass::ControlClass() {
    attitudePID.constants.kp = 30;
    attitudePID.constants.ki = 200;
    attitudePID.constants.kd = 8;

    attitudePID.yawConstants.kp = 40;
    attitudePID.yawConstants.ki = 0;
    attitudePID.yawConstants.kd = 0;
    
    altitudePID.constants.kp = 400;
    altitudePID.constants.ki = 0;
    altitudePID.constants.kd = 100;
    
    alreadySet_ = false;
}

ControlClass::ControlClass(const ControlClass& orig) {
}

ControlClass::~ControlClass() {
}

void ControlClass::update() {
    
    attitudeControl_();
    evaluateAltitudeControl_(); //Checks to see if altitude control if required, and performs if necessary
    updatePWM_();    
}

inline void ControlClass::updatePWM_(){
    PICInterface.pwmwidths.frontleft = ((PICInterface.rx.throttle * (MOTOR_MAX - MOTOR_MIN)) + MOTOR_MIN) - attitudePID.output.phi - attitudePID.output.psi + attitudePID.output.theta + altitudePID.output;
    PICInterface.pwmwidths.frontright = ((PICInterface.rx.throttle * (MOTOR_MAX - MOTOR_MIN)) + MOTOR_MIN) + attitudePID.output.phi - attitudePID.output.psi - attitudePID.output.theta + altitudePID.output;
    PICInterface.pwmwidths.rearright = ((PICInterface.rx.throttle * (MOTOR_MAX - MOTOR_MIN)) + MOTOR_MIN) + attitudePID.output.phi + attitudePID.output.psi + attitudePID.output.theta + altitudePID.output;
    PICInterface.pwmwidths.rearleft = ((PICInterface.rx.throttle * (MOTOR_MAX - MOTOR_MIN)) + MOTOR_MIN) - attitudePID.output.phi + attitudePID.output.psi - attitudePID.output.theta + altitudePID.output;
    PICInterface.setPWM();
}

inline void ControlClass::attitudeControl_() {
    attitudePID.prevError = attitudePID.error;

    attitudePID.error.phi = PICInterface.rx.pitch - AHRS.orientation.phi;
    attitudePID.error.psi = PICInterface.rx.roll - AHRS.orientation.psi;
    attitudePID.error.theta = PICInterface.rx.yaw - AHRS.calibratedData.r; //Yaw uses rate control instead of angle

    attitudePID.differential.phi = (attitudePID.error.phi - attitudePID.prevError.phi) / Timer.dt;
    attitudePID.differential.psi = (attitudePID.error.psi - attitudePID.prevError.psi) / Timer.dt;
    attitudePID.differential.theta = (attitudePID.error.theta - attitudePID.prevError.theta) / Timer.dt;

    attitudePID.integral.phi += attitudePID.error.phi * Timer.dt;
    attitudePID.integral.psi += attitudePID.error.psi * Timer.dt;
    attitudePID.integral.theta += attitudePID.error.theta * Timer.dt;

    constrain_(&attitudePID.integral.phi, 0.1);
    constrain_(&attitudePID.integral.psi, 0.1);
    constrain_(&attitudePID.integral.theta, 0.1);

    //Moving average filter on differential term
    static int i;
    differentialBuf[i] = attitudePID.differential;
    i++;
    if(i == differentialFilterSize) {
	i = 0;
    }
    attitudePID.differential.phi = 0;
    attitudePID.differential.psi = 0;
    attitudePID.differential.theta = 0;
    for(int x = 0; x < differentialFilterSize; x++) {
	attitudePID.differential.phi += differentialBuf[x].phi;
	attitudePID.differential.psi += differentialBuf[x].psi;
	attitudePID.differential.theta += differentialBuf[x].theta;
    }
    attitudePID.differential.phi /= differentialFilterSize;
    attitudePID.differential.psi /= differentialFilterSize;
    attitudePID.differential.theta /= differentialFilterSize;

    attitudePID.output.phi = attitudePID.constants.kp * attitudePID.error.phi + attitudePID.constants.kd * attitudePID.differential.phi + attitudePID.constants.ki * attitudePID.integral.phi;
    attitudePID.output.psi = attitudePID.constants.kp * attitudePID.error.psi + attitudePID.constants.kd * attitudePID.differential.psi + attitudePID.constants.ki * attitudePID.integral.psi;
    attitudePID.output.theta = attitudePID.yawConstants.kp * attitudePID.error.theta; // + yawPIDConstants.kd * differential.theta;

}

inline void ControlClass::evaluateAltitudeControl_() {
    if(PICInterface.rxWidths.sw2 > 15000 && alreadySet_ == false) {
	targetAltitude_ = AHRS.calibratedData.altitude;
	alreadySet_ = true;
	altitudeControl_();
	std::cout << "Altitude hold active at " << targetAltitude_ << "m" << std::endl;
    } else if(PICInterface.rxWidths.sw2 > 15000 && alreadySet_ == true) {
	altitudeControl_();
    } else if(PICInterface.rxWidths.sw2 < 15000) {
	alreadySet_ = false;
	altitudePID.output = 0;
    }
}

inline void ControlClass::altitudeControl_() {
    altitudePID.prevError = altitudePID.error;
    altitudePID.error = targetAltitude_ - AHRS.calibratedData.altitude;
    altitudePID.differential = (altitudePID.error - altitudePID.prevError) / Timer.dt;
    altitudePID.output = altitudePID.constants.kp * altitudePID.error + altitudePID.differential * altitudePID.error;
    constrain_(&altitudePID.output, 500);
}

inline void ControlClass::constrain_(double* value, float range) {
    if(*value > range) *value = range;
    else if(*value < -range) *value = -range;
}

void ControlClass::setAttitudePID(int p, int i, int d) {
    attitudePID.constants.kp = p;
    attitudePID.constants.ki = i;
    attitudePID.constants.kd = d;
}

void ControlClass::getAttitudePID() {
    std::cout << attitudePID.constants.kp << ", " << attitudePID.constants.ki << ", " << attitudePID.constants.kd << std::endl;
}

void ControlClass::setYawPID(int p, int i, int d) {
    attitudePID.yawConstants.kp = p;
    attitudePID.yawConstants.ki = i;
    attitudePID.yawConstants.kd = d;
}

void ControlClass::getYawPID() {
    std::cout << attitudePID.yawConstants.kp << ", " << attitudePID.yawConstants.ki << ", " << attitudePID.yawConstants.kd << std::endl;
}