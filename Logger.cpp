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
std::fstream Log;

LoggerClass::LoggerClass()
{
    logging = false;
    sampleno = 0;
}

LoggerClass::LoggerClass(const LoggerClass& orig)
{
}

LoggerClass::~LoggerClass()
{
    Log.close();
}

void LoggerClass::open(const char* filename)
{
    Log.open(filename, std::fstream::out);
    Log.rdbuf()->pubsetbuf(0, 0); //No buffer
    logging = true;
}

void LoggerClass::update()
{
    sampleno++;
    if (logging)
    {
	Log << sampleno << ", "
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
		<< AHRS.accelAngles.phi << ", "
		<< AHRS.accelAngles.psi << ", "
		<< AHRS.orientation.phi << ", "
		<< AHRS.orientation.psi << ", "
		<< AHRS.orientation.theta << ", "
		<< PICInterface.rx.pitch << ", "
		<< PICInterface.rx.pitchrate << ", "
		<< PICInterface.rx.roll << ", "
		<< PICInterface.rx.rollrate << ", "
		<< PICInterface.rx.throttle << ", "
		<< PICInterface.rx.yawrate << ", "
		<< PICInterface.pwmwidths.frontleft << ", "
		<< PICInterface.pwmwidths.frontright << ", "
		<< PICInterface.pwmwidths.rearleft << ", "
		<< PICInterface.pwmwidths.rearright << ", "
		<< Control.ratePitchPID.output << ", "
		<< Control.rateRollPID.output << ", "
		<< Control.rateYawPID.output << ", "
		<< Control.attitudePitchPID.output << ", "
		<< Control.attitudeRollPID.output << ", "
		//		//Add additional logs below
		<< std::endl;
    }
}