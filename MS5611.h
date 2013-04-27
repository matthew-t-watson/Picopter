/* 
 * File:   MS5611.h
 * Author: matt
 *
 * Created on 12 February 2013, 17:01
 */

#ifndef MS5611_H
#define	MS5611_H

#include <iostream>
#include "I2CInterface.h"

const uint8_t MS5611_ADDRESS = 0x77;
const uint8_t C0_ADDRESS = 0xA0;
const uint8_t C1_ADDRESS = 0xA2;
const uint8_t C2_ADDRESS = 0xA4;
const uint8_t C3_ADDRESS = 0xA6;
const uint8_t C4_ADDRESS = 0xA8;
const uint8_t C5_ADDRESS = 0xAA;
const uint8_t C6_ADDRESS = 0xAC;
const uint8_t CRC_ADDRESS = 0xAE;
const uint8_t D1Conv4096 = 0x48;
const uint8_t D2Conv4096 = 0x58;
const uint8_t resetCommand = 0x1E;

class MS5611Class
{
public:
    MS5611Class();
    MS5611Class(const MS5611Class& orig);
    virtual ~MS5611Class();
    void getPressure(int32_t *pressure);
private:
    uint8_t buf_[16];
    void getCalib_();
    void startTempConversion_();
    void startPressureConversion_();
    void getRawPressure_();
    void getRawTemperature_();
    void calculatePressure_(int32_t *pressure);
    void reset_();

    uint16_t C1_, C2_, C3_, C4_, C5_, C6_, CRC_;
    uint32_t D1_, D2_;
    int32_t dT_, TEMP_, P_;
    int64_t OFF_, SENS_;

    enum e_lastConv_
    {
        Temperature,
        Pressure
    } lastConv_;
};

extern MS5611Class MS5611;
#endif	/* MS5611_H */

