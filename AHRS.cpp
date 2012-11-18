/* 
 * File:   AHRS.cpp
 * Author: matt
 * 
 * Created on 29 October 2012, 22:38
 */

#include "AHRS.h"
#include "struct_rawData.h"
#include "HMC5883L.h"
#include "unistd.h"

AHRSClass AHRS;

AHRSClass::AHRSClass()
{
    //Hardcoded for now until config file is fixed
    zeroPoints_.p = -17;
    zeroPoints_.q = -9;
    zeroPoints_.r = -36;
}

AHRSClass::AHRSClass(const AHRSClass& orig)
{
}

AHRSClass::~AHRSClass()
{
}

void AHRSClass::update()
{
    getSensors_();
    calibrateData_();
    DLPF();
    calcAccelAngles_();
    filter_();
}

void AHRSClass::readConfig()
{
    zeroPoints_.x = Config.getValueOfKey<float> ("zero_x");
    zeroPoints_.y = Config.getValueOfKey<float> ("zero_y");
    zeroPoints_.z = Config.getValueOfKey<float> ("zero_z");
    zeroPoints_.p = Config.getValueOfKey<float> ("zero_p");
    zeroPoints_.q = Config.getValueOfKey<float> ("zero_q");
    zeroPoints_.r = Config.getValueOfKey<float> ("zero_r");
}

//Collect data from all the sensors. Currently only the MPU is collected.

void AHRSClass::getSensors_()
{
    MPU6050.getSensors(&rawData_);
    HMC5883L.getField(&rawData_);
}

void AHRSClass::calibrateData_() //Need to add scales and zero points + any required inversions
{
    calibratedData.x = (rawData_.x * (9.81 / 2048.0)) - zeroPoints_.x;
    calibratedData.y = (rawData_.y * (9.81 / 2048.0)) - zeroPoints_.y;
    calibratedData.z = (rawData_.z * (9.81 / 2048.0)) - zeroPoints_.z;
    calibratedData.temp = (rawData_.temp + 521) / 340.0;
    calibratedData.p = (rawData_.p / 16.4) - zeroPoints_.p;
    calibratedData.q = (-rawData_.q / 16.4) - zeroPoints_.q;
    calibratedData.r = (rawData_.r / 16.4) - zeroPoints_.r;

    calibratedData.mag_x = (rawData_.mag_x - zeroPoints_.mag_x) / 1090.0;
    calibratedData.mag_y = (rawData_.mag_y - zeroPoints_.mag_y) / 1090.0;
    calibratedData.mag_z = (rawData_.mag_z - zeroPoints_.mag_z) / 1090.0;
}

void AHRSClass::calcAccelAngles_()
{
    accelAngles.phi = (180 / pi) * atan2(calibratedData.y, sqrt(pow(calibratedData.x, 2) + pow(calibratedData.z, 2)));
    accelAngles.psi = (180 / pi) * atan2(calibratedData.x, sqrt(pow(calibratedData.y, 2) + pow(calibratedData.z, 2)));
}

void AHRSClass::filter_()
{
    const double a = 0.990;
    orientation.phi = (orientation.phi + calibratedData.p * Timer.dt) * a + accelAngles.phi * (1 - a);
    orientation.psi = (orientation.psi + calibratedData.q * Timer.dt) * a + accelAngles.psi * (1 - a);
    orientation.theta += calibratedData.r * Timer.dt;
}

void AHRSClass::DLPF() //3rd order butterworth, 20Hz fc with 400hz samplerate
{
    xv[0] = xv[1];
    xv[1] = xv[2];
    xv[2] = xv[3];
    xv[3] = calibratedData.x / GAIN;
    yv[0] = yv[1];
    yv[1] = yv[2];
    yv[2] = yv[3];
    yv[3] = (xv[0] + xv[3]) + 3 * (xv[1] + xv[2]) + (0.5320753683 * yv[0]) + (-1.9293556691 * yv[1]) + (2.3740947437 * yv[2]);
    calibratedDataFiltered.x = yv[3];
}

void AHRSClass::zeroGyros()
{
    Timer.stop();
    double sums[3] = {0};
    for (int i = 0; i<1000; i++)
    {
	update();
	sums[0] += calibratedData.p;
	sums[1] += calibratedData.p;
	sums[2] += calibratedData.p;
	usleep(2500);
    }
    zeroPoints_.p = sums[0] / 1000;
    zeroPoints_.q = sums[1] / 1000;
    zeroPoints_.r = sums[2] / 1000;
    
    std::cout << "Gyroscopes calibrated with zero points of " << zeroPoints_.p << ", " << zeroPoints_.q << ", " << zeroPoints_.r << std::endl;
    Timer.start();
}

