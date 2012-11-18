/*
 * File:   MPU6050.h
 * Author: matt
 *
 * Created on 22 October 2012, 11:39
 */

#ifndef MPU6050_H
#define	MPU6050_H

#include <iostream>

#include "MPU6050RegDefs.h"
#include "I2CInterface.h"
#include "struct_rawData.h"

class MPU6050Class
{
public:
    MPU6050Class();
    virtual ~MPU6050Class();
    
    bool getSensors(s_rawData* rawData);
    bool checkConnection();
    void initialise();
private: 
    int setSampleRateDivider_(unsigned char value);
    int setDLPFConfig_(unsigned char cutoff);
    int setGyroConfig_(unsigned char config);
    int setAccelConfig_(unsigned char config);
    int setPowerManagement1_(unsigned char config);
    int setPowerManagement2_(unsigned char config);
    void enablePassthrough_();
            
    unsigned char buf_[32];
};

extern MPU6050Class MPU6050;
#endif	/* MPU6050_H */

