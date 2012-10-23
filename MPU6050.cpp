/*
 * File:   MPU6050.cpp
 * Author: matt
 *
 * Created on 22 October 2012, 11:39
 */

#include "MPU6050.h"

MPU6050* MPU6050::p_instance_ = NULL;

MPU6050* MPU6050::instance()
{
    if (!p_instance_)
        p_instance_ = new MPU6050;
}

MPU6050::MPU6050()
{

}

bool MPU6050::getSensors(unsigned char* buf)
{
    return(I2CInterface::instance()->get(MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_H, buf, 14));
}

bool MPU6050::checkConnection()
{
    std::cout << "Checking MPU6050 connection" << std::endl;
    I2CInterface::instance()->get(MPU6050_ADDRESS, MPU6050_RA_WHO_AM_I, buf_, 1);
    if(buf_[0] == MPU6050_ADDRESS)
        return true;
    else
        std::cout << "Expected 0x68, recieved " << buf_[0] << std::endl;
        return false;
}

void MPU6050::initialise_()
{
    setPowerManagement1_(MPU6050_CLOCK_PLL_ZGYRO); //Clear sleep bit and set clock source
    setSampleRateDivider_(0);
    setGyroConfig_(MPU6050_GYRO_FS_2000);
    setAccelConfig_(MPU6050_ACCEL_FS_16);
}

bool MPU6050::setSampleRateDivider_(unsigned char value)
{
    buf_[0] = MPU6050_RA_SMPLRT_DIV;
    buf_[1] = value;
    return(I2CInterface::instance()->send(MPU6050_ADDRESS, buf_, 2));
}

bool MPU6050::setDLPFConfig_(unsigned char cutoff)
{
    buf_[0] = MPU6050_RA_CONFIG;
    buf_[1] = cutoff;
    return(I2CInterface::instance()->send(MPU6050_ADDRESS, buf_, 2));
}

bool MPU6050::setGyroConfig_(unsigned char config)
{
    buf_[0] = MPU6050_RA_GYRO_CONFIG;
    buf_[1] = config;
    return(I2CInterface::instance()->send(MPU6050_ADDRESS, buf_, 2));
}

bool MPU6050::setAccelConfig_(unsigned char config)
{
    buf_[0] = MPU6050_RA_ACCEL_CONFIG;
    buf_[1] = config;
    return(I2CInterface::instance()->send(MPU6050_ADDRESS, buf_, 2));
}

bool MPU6050::setPowerManagement1_(unsigned char config)
{
    buf_[0] = MPU6050_RA_PWR_MGMT_1;
    buf_[1] = config;
    return(I2CInterface::instance()->send(MPU6050_ADDRESS, buf_, 2));
}