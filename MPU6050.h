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



class MPU6050
{
public:
    static MPU6050* instance();
    bool getSensors(unsigned char* buf);
    bool checkConnection();
private:
    MPU6050();
    static MPU6050* p_instance_;
    
    void initialise_();
    bool setSampleRateDivider_(unsigned char value);
    bool setDLPFConfig_(unsigned char cutoff);
    bool setGyroConfig_(unsigned char config);
    bool setAccelConfig_(unsigned char config);
    bool setPowerManagement1_(unsigned char config);
    
    unsigned char buf_[];
};

#endif	/* MPU6050_H */

