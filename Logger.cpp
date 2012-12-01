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

LoggerClass LogMan;
std::fstream Log;

LoggerClass::LoggerClass()
{
    logging = false;
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
    if (logging)
    {
	Log << Timer.dt * 1000 << ", "
		<< AHRS.calibratedData.x << ", "
		<< AHRS.calibratedData.y << ", "
		<< AHRS.calibratedData.z << ", "
		<< AHRS.calibratedData.p << ", "
		<< AHRS.calibratedData.q << ", "
		<< AHRS.calibratedData.r << ", "
		<< AHRS.calibratedData.temp << ", "
		<< AHRS.calibratedData.mag_x << ", "
		<< AHRS.calibratedData.mag_y << ", "
		<< AHRS.calibratedData.mag_z << ", "
		<< AHRS.accelAngles.phi << ", "
		<< AHRS.accelAngles.psi << ", "
		<< AHRS.orientation.phi << ", "
		<< AHRS.orientation.psi << ", "
		<< AHRS.orientation.theta << ", "
		<< PICInterface.rx.pitch << ", "
		<< PICInterface.rx.roll << ", "
		<< PICInterface.rx.throttle << ", "
		<< PICInterface.rx.yaw << ", "
		//		//Add additional logs belowr
		<< std::endl;
    }
}