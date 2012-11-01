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

class MPU6050Class
{
public:
    bool getSensors(s_rawData* rawData);
    bool checkConnection();
    MPU6050Class();
    virtual ~MPU6050Class();
private: 
    void initialise_();
    bool setSampleRateDivider_(unsigned char value);
    bool setDLPFConfig_(unsigned char cutoff);
    bool setGyroConfig_(unsigned char config);
    bool setAccelConfig_(unsigned char config);
    bool setPowerManagement1_(unsigned char config);
    unsigned char buf_[32];
};

extern MPU6050Class MPU6050Interface;
#endif	/* MPU6050_H */

