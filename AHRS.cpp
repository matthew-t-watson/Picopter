/*
 * File:   AHRS.cpp
 * Author: matt
 *
 * Created on 29 October 2012, 22:38
 */

#include "AHRS.h"
#include "HMC5883L.h"
#include "MS5611.h"
#include "unistd.h"

//Values calculated from matlab script MgnCalibration
const double accelEllipsoid00_ = 0.1007;
const double accelEllipsoid01_ = -0.001;
const double accelEllipsoid02_ = -0.0001;
const double accelEllipsoid11_ = -0.1021;
const double accelEllipsoid12_ = -0.0004;
const double accelEllipsoid22_ = 0.1006;

//Values calculated from matlab script MgnCalibration
const double magEllipsoid00_ = 1.8925;
const double magEllipsoid01_ = 0.0399;
const double magEllipsoid02_ = 0.0132;
const double magEllipsoid11_ = 1.8375;
const double magEllipsoid12_ = 0.0474;
const double magEllipsoid22_ = 2.1528;

AHRSClass AHRS;

AHRSClass::AHRSClass() {
    zeroPoints_.x = 0.1786;
    zeroPoints_.y = 0.1545;
    zeroPoints_.z = 0.0911;

    //Values calculated from matlab script MgnCalibration
    zeroPoints_.magx = 0.0576;
    zeroPoints_.magy = -0.0929;
    zeroPoints_.magz = -0.0092;
}

AHRSClass::AHRSClass(const AHRSClass& orig) {
}

AHRSClass::~AHRSClass() {
}

void AHRSClass::update() {
    getSensors_();
    calibrateData_();
    temperatureCompensate_();
    calcAccelAngles_(&calibratedData, &accelAngles);
    fuse_();
}

void AHRSClass::readConfig() {
    zeroPoints_.x = Config.getValueOfKey<float> ("zero_x");
    zeroPoints_.y = Config.getValueOfKey<float> ("zero_y");
    zeroPoints_.z = Config.getValueOfKey<float> ("zero_z");
    zeroPoints_.p = Config.getValueOfKey<float> ("zero_p");
    zeroPoints_.q = Config.getValueOfKey<float> ("zero_q");
    zeroPoints_.r = Config.getValueOfKey<float> ("zero_r");
}

//Collect data from all the sensors. Currently only the MPU is collected.

void AHRSClass::getSensors_() {
    MPU6050.getSensors(&rawData_);
    HMC5883L.getField(&rawData_);
    MS5611.getPressure(&rawData_.pressure);
}

void AHRSClass::calibrateData_() {
    calibratedData.x = (rawData_.x * (9.81 / 4096.0));
    calibratedData.y = (rawData_.y * (9.81 / 4096.0));
    calibratedData.z = (rawData_.z * (9.81 / 4096.0));
    calibratedData.temp = (rawData_.temp + 12412) / 340.0;
    calibratedData.p = (rawData_.p / 65.5);
    calibratedData.q = (rawData_.q / 65.5);
    calibratedData.r = (rawData_.r / 65.5);
    calibratedData.magx = rawData_.mag_x / 1090.0;
    calibratedData.magy = rawData_.mag_y / 1090.0;
    calibratedData.magz = rawData_.mag_z / 1090.0;
    calibratedData.pressure = rawData_.pressure; //Pascals
    calibratedData.altitude = ((-8.31447 * 288.15) / (9.80665 * 0.0289644)) * log(calibratedData.pressure / 101325);

    calibratedData.q = -calibratedData.q;


    //Accelerometer scale and bias correction
    static double acceltemp[3];
    acceltemp[0] = calibratedData.x - zeroPoints_.x;
    acceltemp[1] = calibratedData.y - zeroPoints_.y;
    acceltemp[2] = calibratedData.z - zeroPoints_.z;
    calibratedData.x = accelEllipsoid00_ * acceltemp[0] + accelEllipsoid01_ * acceltemp[1] + accelEllipsoid02_ * acceltemp[2];
    calibratedData.y = accelEllipsoid11_ * acceltemp[1] + accelEllipsoid12_ * acceltemp[2];
    calibratedData.z = accelEllipsoid22_ * acceltemp[2];

    calibratedData.y = -calibratedData.y; //Calibration elipsoid is somehow inverting y axis, need to fix

    //Magnetometer scale and bias correction
    static double magtemp[3];
    magtemp[0] = calibratedData.magx - zeroPoints_.magx;
    magtemp[1] = calibratedData.magy - zeroPoints_.magy;
    magtemp[2] = calibratedData.magz - zeroPoints_.magz;
    calibratedData.magx = magEllipsoid00_ * magtemp[0] + magEllipsoid01_ * magtemp[1] + magEllipsoid02_ * magtemp[2];
    calibratedData.magy = magEllipsoid11_ * magtemp[1] + magEllipsoid12_ * magtemp[2];
    calibratedData.magz = magEllipsoid22_ * magtemp[2];

    //Altitude LPF
#define LENGTH 20
    static int i = 0;
    static double mvAvg[LENGTH] = {0};
    mvAvg[i] = calibratedData.altitude;
    calibratedData.altitude = 0;
    for(int k = 0; k < LENGTH; k++) {
	calibratedData.altitude += mvAvg[k];
    }
    calibratedData.altitude /= LENGTH;
    i++;
    if(i == LENGTH) {
	i = 0;
    }
}

inline void AHRSClass::temperatureCompensate_() {
    static double tempPow1 = calibratedData.temp;
    static double tempPow2 = pow(calibratedData.temp, 2);
    static double tempPow3 = pow(calibratedData.temp, 3);
    static double tempPow4 = pow(calibratedData.temp, 4);
    //Coefficients calculated from freezetest4, 4th degree polynomial
    calibratedData.p -= 8.4877e-9 * tempPow4 + 6.4219e-6 * tempPow3 + 2.5782e-4 * tempPow2 - 0.0041145 * tempPow1 - 1.2974;
    calibratedData.q -= 5.863e-8 * tempPow4 - 5.9746e-6 * tempPow3 + 5.1324e-5 * tempPow2 + 0.0079355 * tempPow1 + 0.59859;
    calibratedData.r -= 4.4929e-8 * tempPow4 - 1.6137e-7 * tempPow3 + 4.8876e-5 * tempPow2 + 0.021246 * tempPow1 - 2.9723;
    calibratedData.x -= -2.8664e-6 * tempPow2 + 4.9565e-4 * tempPow1; // - 0.0011611;
    calibratedData.y -= 1.2728e-6 * tempPow2 + 6.5989e-6 * tempPow1; // + 0.025702;
    calibratedData.z -= 1.6966e-5 * tempPow2 - 0.0035421 * tempPow1; // + 0.056; //Z axis accel shows huge temperature drift (15% over 40 degrees)
}

void AHRSClass::calcAccelAngles_(s_calibratedData* data, s_euler* angles) {
    angles->phi = (180 / pi) * atan(data->y / sqrt(pow(data->x, 2) + pow(data->z, 2)));
    angles->psi = (180 / pi) * atan(data->x / sqrt(pow(data->y, 2) + pow(data->z, 2)));
}

void AHRSClass::fuse_() {
    if(Timer.dt < 0.02) {
	kalmanPhi_.predict(&calibratedData.p, &orientation.phi, &Timer.dt);
	kalmanPsi_.predict(&calibratedData.q, &orientation.psi, &Timer.dt);
    }
    kalmanPhi_.update(&accelAngles.phi, &orientation.phi);
    kalmanPsi_.update(&accelAngles.psi, &orientation.psi);
    orientation.theta += calibratedData.r * Timer.dt;
}

double AHRSClass::magnitude_(double x, double y, double z) {
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

void AHRSClass::calibrateAccelerometers() //Currently not working as intended
{
#define orientation_number 6
    double accel_means[3][orientation_number] = {0};
    //Gather mean accel data from 6 different orientations
    for(uint8_t orientation_count = 0; orientation_count < 6; orientation_count++) {
	std::cout << "Place vehicle into orientation " << std::dec << orientation_count << " and press enter" << std::endl;
	std::string trash;
	std::getline(std::cin, trash);
	std::cout << "Measuring" << std::endl;

	for(uint16_t sample_number = 0; sample_number < 1000; sample_number++) {
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
    for(uint8_t iteration_count = 0; iteration_count < iteration_number; iteration_count++) {
	for(uint8_t orientation_count = 0; orientation_count < 6; orientation_count++) {
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

void AHRSClass::zeroGyros() {
    //    Timer.stop();
    //    usleep(50000);
    zeroPoints_.p = 0;
    zeroPoints_.q = 0;
    zeroPoints_.r = 0;

    double sums[3] = {0};
    for(int i = 0; i < 100; i++) {
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
