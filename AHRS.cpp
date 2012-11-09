/* 
 * File:   AHRS.cpp
 * Author: matt
 * 
 * Created on 29 October 2012, 22:38
 */

#include "AHRS.h"

AHRSClass AHRS;

AHRSClass::AHRSClass()
{
}

AHRSClass::AHRSClass(const AHRSClass& orig)
{
    readConfig();
}

AHRSClass::~AHRSClass()
{
}

void AHRSClass::update()
{
    getSensors_();
    calibrateData_();
    calcAccelAngles_();
    filter_();
}

void AHRSClass::readConfig()
{
    zeroPoints_.x = Config.getValueOfKey<int16_t>("zero_x");
    zeroPoints_.y = Config.getValueOfKey<int16_t>("zero_y");
    zeroPoints_.z = Config.getValueOfKey<int16_t>("zero_z");
    zeroPoints_.p = Config.getValueOfKey<int16_t>("zero_p");
    zeroPoints_.q = Config.getValueOfKey<int16_t>("zero_q");
    zeroPoints_.r = Config.getValueOfKey<int16_t>("zero_r");
}

//Collect data from all the sensors. Currently only MPU is collected.
void AHRSClass::getSensors_()
{
    MPU6050Interface.getSensors(&rawData_);
}

void AHRSClass::calibrateData_() //Need to add scales and zero points + any required inversions
{
    calibratedData.x = (rawData_.x - zeroPoints_.x) * (9.81 / 2048.0);
    calibratedData.y = (rawData_.y - zeroPoints_.y) * (9.81 / 2048.0);
    calibratedData.z = (rawData_.z - zeroPoints_.z) * (9.81 / 2048.0);
    calibratedData.temp = (rawData_.temp + 521) / 340.0;
    calibratedData.p = (rawData_.p - zeroPoints_.p) / 16.4;
    calibratedData.q = -(rawData_.q - zeroPoints_.q) / 16.4;
    calibratedData.r = (rawData_.r - zeroPoints_.r) / 16.4;
}

void AHRSClass::calcAccelAngles_()
{
    accelAngles.phi = (180/pi)*atan2(calibratedData.y,sqrt(pow(calibratedData.x,2) + pow(calibratedData.z,2)));
    accelAngles.psi = (180/pi)*atan2(calibratedData.x,sqrt(pow(calibratedData.y,2) + pow(calibratedData.z,2)));
}

void AHRSClass::filter_()
{
    #define a 0.99
    orientation.phi = (orientation.phi + calibratedData.p * Timer.dt)*a + accelAngles.phi*(1-a);
    orientation.psi = (orientation.psi + calibratedData.q * Timer.dt)*a + accelAngles.psi*(1-a);
    orientation.theta += calibratedData.r * Timer.dt;
}