/* 
 * File:   AHRS.h
 * Author: matt
 *
 * Created on 29 October 2012, 22:38
 */

#ifndef AHRS_H
#define	AHRS_H

#include <iostream>
#include <stdint.h>
#include <math.h>

#include "Timer.h"
#include "MPU6050.h"
#include "ConfigFile.h"
#include "DLPF.h"
#include "Kalman.h"

#include "struct_rawData.h"
#include "struct_calibratedData.h"
#include "struct_euler.h"

#define pi 3.14159265358979
const double g = 9.81816;




class AHRSClass
{
public:
    AHRSClass();
    AHRSClass(const AHRSClass& orig);
    virtual ~AHRSClass();
    void update();
    void readConfig();
    void zeroGyros();
    void calibrateAccelerometers();
    
    s_calibratedData calibratedData;
    s_rawData rawData_;
    s_euler orientation;
    s_euler accelAngles;
    double filterTimeConstant;
private:
    void getSensors_();
    void calibrateData_();
    void temperatureCompensate_();
    void fuse_();
    double magnitude_(double x, double y, double z);
    void calcAccelAngles_(s_calibratedData* data, s_euler* angles);
    DLPF xDLPF, yDLPF, zDLPF;
    KalmanClass kalmanPhi_, kalmanPsi_;
    
    s_calibratedData zeroPoints_;
};

extern AHRSClass AHRS;

#endif	/* AHRS_H */

