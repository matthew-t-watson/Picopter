/* 
 * File:   AHRS.h
 * Author: matt
 *
 * Created on 29 October 2012, 22:38
 */

#ifndef AHRS_H
#define	AHRS_H

#include <iostream>

#include "Timer.h"
#include "MPU6050.h"
#include "Kalman.h"
#include "ExtendedKalman.h"

#include "struct_rawData.h"
#include "struct_calibratedData.h"
#include "struct_euler.h"
#include "Quaternion.h"

const double g = 9.81816;

class AHRSClass
{
public:
    AHRSClass();
    AHRSClass(const AHRSClass& orig);
    virtual ~AHRSClass();
    void update();
    
    s_calibratedData calibratedData;
    s_rawData rawData_;
    s_euler orientation;
    QuaternionClass quaternion;
private:
    void getSensors_();
    void calibrateData_();
    void temperatureCompensate_();
    void fuse_();
    void quaternionToYPR_(QuaternionClass* q, s_euler* orientation);
    double magnitude_(double x, double y, double z);
//    KalmanClass kalmanPhi_, kalmanPsi_;
    ExtendedKalmanClass EKF;
};

extern AHRSClass AHRS;

#endif	/* AHRS_H */

