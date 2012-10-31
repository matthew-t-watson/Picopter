/*
 * File:   MPU6050.h
 * Author: matt
 *
 * Created on 22 October 2012, 11:39
 */

#ifndef MPU6050_H
#define	MPU6050_H

#include "MPU6050RegDefs.h"
#include "I2CInterface.h"
#include <iostream>

struct s_rawData
{
    int16_t x;
    int16_t y;
    int16_t z;
    int16_t temp;
    int16_t p;
    int16_t q;
    int16_t r;
};

class MPU6050
{
public:
    static bool getSensors(s_rawData* rawData);
    static bool checkConnection();
    MPU6050();
private: 
    static void initialise_();
    static bool setSampleRateDivider_(unsigned char value);
    static bool setDLPFConfig_(unsigned char cutoff);
    static bool setGyroConfig_(unsigned char config);
    static bool setAccelConfig_(unsigned char config);
    static bool setPowerManagement1_(unsigned char config);
};

#endif	/* MPU6050_H */

