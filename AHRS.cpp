/* 
 * File:   AHRS.cpp
 * Author: matt
 * 
 * Created on 29 October 2012, 22:38
 */

#include "AHRS.h"
#include "HMC5883L.h"
#include "unistd.h"

AHRSClass AHRS;

AHRSClass::AHRSClass()
{
    filterTimeConstant = 0.99;
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
//    xDLPF.iterate(&calibratedData.x, &calibratedData.x);
//    yDLPF.iterate(&calibratedData.y, &calibratedData.y);
//    zDLPF.iterate(&calibratedData.z, &calibratedData.z);
    calcAccelAngles_(&calibratedData, &accelAngles);
    temperatureCompensate_();
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
    calibratedData.temp = (rawData_.temp + 12412) / 340.0;
    calibratedData.p = (rawData_.p / 65.5);
    calibratedData.q = (rawData_.q / 65.5);
    calibratedData.r = (rawData_.r / 65.5);
    calibratedData.q = -calibratedData.q;

    calibratedData.mag_x = (rawData_.mag_x - zeroPoints_.mag_x) / 1090.0;
    calibratedData.mag_y = (rawData_.mag_y - zeroPoints_.mag_y) / 1090.0;
    calibratedData.mag_z = (rawData_.mag_z - zeroPoints_.mag_z) / 1090.0;
}

inline void AHRSClass::temperatureCompensate_()
{
    calibratedData.p -= 0.0089 * calibratedData.temp - 1.36;
    calibratedData.q -= 0.0026 * calibratedData.temp + 0.51;
    calibratedData.r -= 0.02405 * calibratedData.temp - 2.823;
}

void AHRSClass::calcAccelAngles_(s_calibratedData* data, s_euler* angles)
{
    angles->phi = (180 / pi) * atan(data->y/ sqrt(pow(data->x, 2) + pow(data->z, 2)));
    angles->psi = (180 / pi) * atan(data->x/ sqrt(pow(data->y, 2) + pow(data->z, 2)));
}

void AHRSClass::filter_()
{
//    orientation.phi = (orientation.phi + calibratedData.p * Timer.dt) * 0.99 + accelAngles.phi * 0.01;
//    orientation.psi = (orientation.psi + calibratedData.q * Timer.dt) * 0.99 + accelAngles.psi * 0.01;
    
    kalmanPhi_.predict(&calibratedData.p, &orientation.phi, &Timer.dt);
    kalmanPsi_.predict(&calibratedData.q, &orientation.psi, &Timer.dt);
    
    double magnitude = magnitude_(calibratedData.x, calibratedData.x, calibratedData.x);
    if(magnitude > 14 || magnitude < 7.14)
    {
	kalmanPhi_.update(&accelAngles.phi, &orientation.phi);
	kalmanPsi_.update(&accelAngles.psi, &orientation.psi);
    }
    
    orientation.theta += calibratedData.r * Timer.dt;
}

double AHRSClass::magnitude_(double x, double y, double z)
{
    return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

void AHRSClass::calibrateAccelerometers()
{
    #define orientation_number 6
    double accel_means[3][orientation_number] = {0};
    //Gather mean accel data from 6 different orientations
    for (uint8_t orientation_count = 0; orientation_count < 6; orientation_count++)
    {
	std::cout << "Place vehicle into orientation " << std::dec << orientation_count << " and press enter" << std::endl;
	std::string trash;
	std::getline(std::cin, trash);
	std::cout << "Measuring" << std::endl;
	
	for (uint16_t sample_number = 0; sample_number < 1000; sample_number++)
	{
	    update();
	    accel_means[0][orientation_count] += calibratedData.x;
	    accel_means[1][orientation_count] += calibratedData.y;
	    accel_means[2][orientation_count] += calibratedData.z;
	}
	accel_means[0][orientation_count] /= 1000;
	accel_means[1][orientation_count] /= 1000;
	accel_means[2][orientation_count] /= 1000;
	
	std::cout << "Measured means of " << accel_means[0][orientation_count] << ", " << accel_means[1][orientation_count] << ", " << accel_means[2][orientation_count] << std::endl;
	//Need to add code to check variance of measurements to verify accuracy
    }
    //accel_means now contains mean xyz accelerations at 6 different orientations
    
    
    //Bias estimation
    float accel_biases[3] = {0};
    float temp3 = 0;
    float temp2[3] = {0};
    float temp1[3] = {0};
#define iteration_number 30
    for (uint8_t iteration_count = 0; iteration_count < iteration_number; iteration_count++)
    {
	for (uint8_t orientation_count = 0; orientation_count < 6; orientation_count++)
	{
	    temp2[0] = accel_means[0][orientation_count] - accel_biases[0];
	    temp2[1] = accel_means[1][orientation_count] - accel_biases[1];
	    temp2[2] = accel_means[2][orientation_count] - accel_biases[2];
	    
	    temp3 = magnitude_(accel_means[0][orientation_count], accel_means[1][orientation_count], accel_means[2][orientation_count]);
	    
	    temp1[0] += accel_means[0][orientation_count] - g * (temp2[0] / temp3);
	    temp1[1] += accel_means[1][orientation_count] - g * (temp2[1] / temp3);
	    temp1[2] += accel_means[2][orientation_count] - g * (temp2[2] / temp3);
	}
	accel_biases[0] = temp1[0] / orientation_number;
	accel_biases[1] = temp1[1] / orientation_number;
	accel_biases[2] = temp1[2] / orientation_number;
	std::cout << "Biases = " << accel_biases[0] << ", " << accel_biases[1] << ", " << accel_biases[2] << std::endl;
    }
    std::cout << "Accelerometers calibrated with biases " << accel_biases[0] << ", " << accel_biases[1] << ", " << accel_biases[2] << std::endl;
    
    zeroPoints_.x = accel_biases[0];
    zeroPoints_.y = accel_biases[1];
    zeroPoints_.z = accel_biases[2];
    
}

void AHRSClass::zeroGyros()
{
//    Timer.stop();
//    usleep(50000);
    zeroPoints_.p = 0;
    zeroPoints_.q = 0;
    zeroPoints_.r = 0;
    
    double sums[3] = {0};
    for (int i = 0; i < 100; i++)
    {
	getSensors_();
	calibrateData_();
	sums[0] += calibratedData.p;
	sums[1] += calibratedData.q;
	sums[2] += calibratedData.r;
	usleep(10000);
    }
    zeroPoints_.p = sums[0] / 100;
    zeroPoints_.q = sums[1] / 100;
    zeroPoints_.r = sums[2] / 100;

    std::cout << "Gyroscopes calibrated with zero points of " << zeroPoints_.p << ", " << zeroPoints_.q << ", " << zeroPoints_.r << std::endl;
    Timer.start();
}

