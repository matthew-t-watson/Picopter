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
    ratePitchPID.initialise(16, 20, 0, 5, 1500, 1);
    rateRollPID.initialise(16, 20, 0, 5, 1500, 1);
    rateYawPID.initialise(40, 20, 0, 5, 1500, 1);
    attitudePitchPID.initialise(4, 1, 0.02, 1, 1000, 16, &AHRS.calibratedData.p);
    attitudeRollPID.initialise(4, 1, 0.02, 1, 1000, 16, &AHRS.calibratedData.q);

    altitudeHoldActive_ = false;
    motorTesting_ = false;
}

ControlClass::ControlClass(const ControlClass& orig) {
}

ControlClass::~ControlClass() {
}

void ControlClass::update() {
    if(motorTesting_ == false) {
	if(PICInterface.rx.sw2 == false) {//in rate mode
	    rateControl_(&PICInterface.rx.pitchRateDem, &PICInterface.rx.rollRateDem, &PICInterface.rx.yawRateDem);
	} else if(PICInterface.rx.sw2 == true) { //in attitude mode
	    attitudeControl_(&PICInterface.rx.pitchDem, &PICInterface.rx.rollDem, &PICInterface.rx.yawRateDem);
	}
    } else {
	incrementMotorTest_();
    }
    //        evaluateAltitudeControl_(); //Checks to see if altitude control if required, and performs if necessary
}

void ControlClass::startMotorTest() {
    std::cout << "Starting motor test" << std::endl;
    motorTestCounter_ = 0;
    motorTesting_ = true;
}

void ControlClass::incrementMotorTest_() {
    switch(motorTestCounter_ / 1000) {
	case 0:
	    PICInterface.pwmwidths.frontleft = 9000;
	    PICInterface.pwmwidths.frontright = 9000;
	    PICInterface.pwmwidths.rearright = 9000;
	    PICInterface.pwmwidths.rearleft = 9000;
	    PICInterface.setPWM();
	    break;
	case 1:
	    PICInterface.pwmwidths.frontleft = 14000;
	    PICInterface.pwmwidths.frontright = 9000;
	    PICInterface.pwmwidths.rearright = 9000;
	    PICInterface.pwmwidths.rearleft = 9000;
	    PICInterface.setPWM();
	    break;
	case 2:
	    PICInterface.pwmwidths.frontleft = 9000;
	    PICInterface.pwmwidths.frontright = 9000;
	    PICInterface.pwmwidths.rearright = 9000;
	    PICInterface.pwmwidths.rearleft = 9000;
	    PICInterface.setPWM();
	    break;
	case 3:
	    PICInterface.pwmwidths.frontleft = 9000;
	    PICInterface.pwmwidths.frontright = 14000;
	    PICInterface.pwmwidths.rearright = 9000;
	    PICInterface.pwmwidths.rearleft = 9000;
	    PICInterface.setPWM();
	    break;
	case 4:
	    PICInterface.pwmwidths.frontleft = 9000;
	    PICInterface.pwmwidths.frontright = 9000;
	    PICInterface.pwmwidths.rearright = 9000;
	    PICInterface.pwmwidths.rearleft = 9000;
	    PICInterface.setPWM();
	    break;
	case 5:
	    PICInterface.pwmwidths.frontleft = 9000;
	    PICInterface.pwmwidths.frontright = 9000;
	    PICInterface.pwmwidths.rearright = 14000;
	    PICInterface.pwmwidths.rearleft = 9000;
	    PICInterface.setPWM();
	    break;
	case 6:
	    PICInterface.pwmwidths.frontleft = 9000;
	    PICInterface.pwmwidths.frontright = 9000;
	    PICInterface.pwmwidths.rearright = 9000;
	    PICInterface.pwmwidths.rearleft = 9000;
	    PICInterface.setPWM();
	    break;
	case 7:
	    PICInterface.pwmwidths.frontleft = 9000;
	    PICInterface.pwmwidths.frontright = 9000;
	    PICInterface.pwmwidths.rearright = 9000;
	    PICInterface.pwmwidths.rearleft = 14000;
	    PICInterface.setPWM();
	    break;
	case 8:
	    PICInterface.pwmwidths.frontleft = 9000;
	    PICInterface.pwmwidths.frontright = 9000;
	    PICInterface.pwmwidths.rearright = 9000;
	    PICInterface.pwmwidths.rearleft = 9000;
	    PICInterface.setPWM();
	    motorTesting_ = false;
	    std::cout << "Motor test complete" << std::endl;
	    break;
    }
    motorTestCounter_++;
}

inline void ControlClass::updatePWM_(float* throttle, float* pitch, float* roll, float* yaw) {
    PICInterface.pwmwidths.frontleft = ((*throttle * (MOTOR_MAX - MOTOR_MIN)) + MOTOR_MIN) - *pitch + *roll - *yaw + altitudePID.output;
    PICInterface.pwmwidths.frontright = ((*throttle * (MOTOR_MAX - MOTOR_MIN)) + MOTOR_MIN) - *pitch - *roll + *yaw + altitudePID.output;
    PICInterface.pwmwidths.rearright = ((*throttle * (MOTOR_MAX - MOTOR_MIN)) + MOTOR_MIN) + *pitch - *roll - *yaw + altitudePID.output;
    PICInterface.pwmwidths.rearleft = ((*throttle * (MOTOR_MAX - MOTOR_MIN)) + MOTOR_MIN) + *pitch + *roll + *yaw + altitudePID.output;
    PICInterface.setPWM();
}

void ControlClass::rateControl_(float* pitchrate, float* rollrate, float* yawrate) {
    ratePitchPID.calculate(&AHRS.calibratedData.p, pitchrate, &Timer.dt);
    rateRollPID.calculate(&AHRS.calibratedData.q, rollrate, &Timer.dt);
    rateYawPID.calculate(&AHRS.calibratedData.r, yawrate, &Timer.dt);
    updatePWM_(&PICInterface.rx.throttleDem, &ratePitchPID.output, &rateRollPID.output, &rateYawPID.output);
}

void ControlClass::attitudeControl_(float* targetPitch, float* targetRoll, float* targetYawRate) {
    attitudePitchPID.calculate(&AHRS.orientation.pitch, targetPitch, &Timer.dt);
    attitudeRollPID.calculate(&AHRS.orientation.roll, targetRoll, &Timer.dt);
    rateControl_(&attitudePitchPID.output, &attitudeRollPID.output, targetYawRate);
}

void ControlClass::setRatePID(float KP, float KI, float KD) {
    ratePitchPID.setPID(KP, KI, KD);
    rateRollPID.setPID(KP, KI, KD);
}

void ControlClass::getRatePID() {
    ratePitchPID.getPID();
    rateRollPID.getPID();
}

void ControlClass::setAttitudePID(float KP, float KI, float KD) {
    attitudePitchPID.setPID(KP, KI, KD);
    attitudeRollPID.setPID(KP, KI, KD);
}

void ControlClass::getAttitudePID() {
    attitudePitchPID.getPID();
    attitudeRollPID.getPID();
}

void ControlClass::evaluateAltitudeControl_() {
    if(PICInterface.rxWidths.sw2 > 15000 && altitudeHoldActive_ == false) {
	targetAltitude_ = AHRS.calibratedData.altitude;
	altitudeHoldActive_ = true;
	altitudeControl_();
	std::cout << "Altitude hold active at " << targetAltitude_ << "m" << std::endl;
    } else if(PICInterface.rxWidths.sw2 > 15000 && altitudeHoldActive_ == true) {
	altitudeControl_();
    } else if(PICInterface.rxWidths.sw2 < 15000) {
	altitudeHoldActive_ = false;
	altitudePID.output = 0;
    }
}

void ControlClass::altitudeControl_() {
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

