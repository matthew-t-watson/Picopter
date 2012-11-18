/* 
 * File:   HMC5883L.h
 * Author: matt
 *
 * Created on 16 November 2012, 22:44
 */

#ifndef HMC5883L_H
#define	HMC5883L_H

#include "struct_rawData.h"

const uint8_t HMC5883L_ADDRESS = 0x1E;

const uint8_t HMC5883L_RA_CONFIG_A = 0x00;
const uint8_t HMC5883L_RA_CONFIG_B = 0x01;
const uint8_t HMC5883L_RA_MODE = 0x02;
const uint8_t HMC5883L_RA_X_H = 0x03;
const uint8_t HMC5883L_RA_X_L = 0x04;
const uint8_t HMC5883L_RA_Z_H = 0x05;
const uint8_t HMC5883L_RA_Z_L = 0x06;
const uint8_t HMC5883L_RA_Y_H = 0x07;
const uint8_t HMC5883L_RA_Y_L = 0x08;
const uint8_t HMC5883L_RA_STATUS = 0x09;
const uint8_t HMC5883L_RA_ID_A = 0x10;
const uint8_t HMC5883L_RA_ID_B = 0x11;
const uint8_t HMC5883L_RA_ID_C = 0x12;

class HMC5883LClass
{
public:
    HMC5883LClass();
    HMC5883LClass(const HMC5883LClass& orig);
    virtual ~HMC5883LClass();
    
    void initialise();
    void getField(s_rawData* rawData);
    
private:
    bool setConfigA_(uint8_t value);
    bool setConfigB_(uint8_t value);
    bool setMode_(uint8_t value);
    bool checkCommunication_();
};

extern HMC5883LClass HMC5883L;

#endif	/* HMC5883L_H */

