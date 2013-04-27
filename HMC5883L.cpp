/* 
 * File:   HMC5883L.cpp
 * Author: matt
 * 
 * Created on 16 November 2012, 22:44
 */

#include "HMC5883L.h"
#include "I2CInterface.h"

HMC5883LClass HMC5883L;

HMC5883LClass::HMC5883LClass()
{
}

HMC5883LClass::HMC5883LClass(const HMC5883LClass& orig)
{
}

HMC5883LClass::~HMC5883LClass()
{
}

void HMC5883LClass::getField(s_rawData* rawData)
{
    uint8_t buf[6];
    I2CInterface.readRegister(HMC5883L_ADDRESS, HMC5883L_RA_X_H, buf, 6);
    rawData->mag_x = static_cast<int16_t>((buf[0] << 8) | buf[1]);
    rawData->mag_z = static_cast<int16_t>((buf[2] << 8) | buf[3]);
    rawData->mag_y = static_cast<int16_t>((buf[4] << 8) | buf[5]);
}

void HMC5883LClass::initialise()
{
    checkCommunication_();
    setConfigA_(0b00011000); //No averaging, 75Hz update rate, no bias
    setConfigB_(0b00100000); //+-1.3 gauss range, 1090 LSB/gauss
    setMode_(0); //Continuous measurement mode
}

bool HMC5883LClass::setConfigA_(uint8_t value)
{
    return I2CInterface.writeRegister(HMC5883L_ADDRESS, HMC5883L_RA_CONFIG_A, &value, 1);
}

bool HMC5883LClass::setConfigB_(uint8_t value)
{
    return I2CInterface.writeRegister(HMC5883L_ADDRESS, HMC5883L_RA_CONFIG_B, &value, 1);
}

bool HMC5883LClass::setMode_(uint8_t value)
{
    return I2CInterface.writeRegister(HMC5883L_ADDRESS, HMC5883L_RA_MODE, &value, 1);
}

bool HMC5883LClass::checkCommunication_()
{
    uint8_t buf[3];
    I2CInterface.readRegister(HMC5883L_ADDRESS, HMC5883L_RA_ID_A, buf, 3);
    if (buf[0] != 'H' | buf[1] != '4' | buf[2] != '3')
    {
	std::cout << "HMC5883L communication failed, recieved " << buf[0] << ", " << buf[1] << ", " << buf[2] << std::endl;
	return false;
    }
    return true;
}


