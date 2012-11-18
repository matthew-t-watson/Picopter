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

#define pi 3.14159265358979

struct s_calibratedData
{
    double x;
    double y;
    double z;
    double temp;
    double p;
    double q;
    double r;
    double mag_x;
    double mag_y;
    double mag_z;
};

struct s_euler
{
    double phi;
    double psi;
    double theta;
};

class AHRSClass
{
public:
    AHRSClass();
    AHRSClass(const AHRSClass& orig);
    virtual ~AHRSClass();
    void update();
    void readConfig();
    void zeroGyros();
    
    s_calibratedData calibratedData;
    s_calibratedData calibratedDataFiltered;
    s_rawData rawData_;
    s_euler orientation;
    s_euler accelAngles;
private:
    void getSensors_();
    void calibrateData_();
    void filter_();
    void calcAccelAngles_();
    
    s_calibratedData zeroPoints_;
    
    void DLPF();
#define NZEROS 3
#define NPOLES 3
#define GAIN   3.450423889e+02
    double xv[NZEROS + 1], yv[NPOLES + 1];
};

extern AHRSClass AHRS;

#endif	/* AHRS_H */

