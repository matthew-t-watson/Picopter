/* 
 * File:   Logger.cpp
 * Author: matt
 * 
 * Created on 01 November 2012, 22:49
 */

#include "Logger.h"
#include "Timer.h"
#include "AHRS.h"
#include "PICInterface.h"
#include "Control.h"

LoggerClass LogMan;

LoggerClass::LoggerClass() {
    logging = false;
    sampleno = 0;
    log.str().reserve(50 * 1000 * 1000); //Allocate 50mb
}

LoggerClass::LoggerClass(const LoggerClass& orig) {
}

LoggerClass::~LoggerClass() {
    logFile.close();
}

void LoggerClass::open(const char* filename) {
    logFile.open(filename, std::fstream::out);
    logFile.rdbuf()->pubsetbuf(0, 0); //No buffer
    logging = true;
}

void LoggerClass::flush() {
    if(timeSinceLastFlush > 1000) {
	logFile << log.str();
	log.str(std::string()); //Clear log
	sync(); //Force dirty page write
	timeSinceLastFlush = 0;
    } else {
	timeSinceLastFlush++;
    }
}

void LoggerClass::update() {
    if(logging) {
	sampleno++;
	log << sampleno << ", "
		<< Timer.dt * 1000 << ", "
		<< AHRS.calibratedData.x << ", "
		<< AHRS.calibratedData.y << ", "
		<< AHRS.calibratedData.z << ", "
		<< AHRS.calibratedData.p << ", "
		<< AHRS.calibratedData.q << ", "
		<< AHRS.calibratedData.r << ", "
		<< AHRS.calibratedData.temp << ", "
		<< AHRS.calibratedData.magx << ", "
		<< AHRS.calibratedData.magy << ", "
		<< AHRS.calibratedData.magz << ", "
		<< AHRS.calibratedData.pressure << ", "
		<< AHRS.calibratedData.altitude << ", "
		<< AHRS.accelAngles.pitch << ", "
		<< AHRS.accelAngles.roll << ", "
		<< AHRS.orientation.pitch << ", "
		<< AHRS.orientation.roll << ", "
		<< AHRS.orientation.yaw << ", "
		<< PICInterface.rx.pitch << ", "
		<< PICInterface.rx.pitchrate << ", "
		<< PICInterface.rx.roll << ", "
		<< PICInterface.rx.rollrate << ", "
		<< PICInterface.rx.throttle << ", "
		<< PICInterface.rx.yawrate << ", "
		<< PICInterface.rx.sw1 << ", "
		<< PICInterface.rx.sw2 << ", "
		<< PICInterface.pwmwidths.frontleft << ", "
		<< PICInterface.pwmwidths.frontright << ", "
		<< PICInterface.pwmwidths.rearleft << ", "
		<< PICInterface.pwmwidths.rearright << ", "
		<< Control.ratePitchPID.output << ", "
		<< Control.rateRollPID.output << ", "
		<< Control.rateYawPID.output << ", "
		<< Control.attitudePitchPID.output << ", "
		<< Control.attitudeRollPID.output << ", "
		<< AHRS.quaternion.w << ", "
		<< AHRS.quaternion.x << ", "
		<< AHRS.quaternion.y << ", "
		<< AHRS.quaternion.z
		//		//Add additional logs below
		<< std::endl;
	if(PICInterface.rx.sw1 == false) {
	    flush();
	}
    }
}