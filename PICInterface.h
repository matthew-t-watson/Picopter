/* 
 * File:   PICInterface.h
 * Author: matt
 *
 * Created on 28 October 2012, 12:53
 */

#ifndef PICINTERFACE_H
#define	PICINTERFACE_H

#include <iostream>
#include <stdint.h>

#include "I2CInterface.h"

const uint8_t PIC_ADDRESS = 0x50;
const uint8_t NUMBER_PWM_CHANNELS = 6;
const uint8_t NUMBER_RX_CHANNELS = 6;

//Simulated register names and addresses
const uint8_t REG_PWM1H = 0;
const uint8_t REG_PWM1L = 1;
const uint8_t REG_PWM2H = 2;
const uint8_t REG_PWM2L = 3;
const uint8_t REG_PWM3H = 4;
const uint8_t REG_PWM3L = 5;
const uint8_t REG_PWM4H = 6;
const uint8_t REG_PWM4L = 7;
const uint8_t REG_PWM5H = 8;
const uint8_t REG_PWM5L = 9;
const uint8_t REG_PWM6H = 10;
const uint8_t REG_PWM6L = 11;
const uint8_t REG_PWMFIRE = 12;
const uint8_t REG_RX1H = 13;
const uint8_t REG_RX1L = 14;
const uint8_t REG_RX2H = 15;
const uint8_t REG_RX2L = 16;
const uint8_t REG_RX3H = 17;
const uint8_t REG_RX3L = 18;
const uint8_t REG_RX4H = 19;
const uint8_t REG_RX4L = 20;
const uint8_t REG_RX5H = 21;
const uint8_t REG_RX5L = 22;
const uint8_t REG_RX6H = 23;
const uint8_t REG_RX6L = 24;


class PICInterface {
public:
    static PICInterface* instance();
    void setPWM(uint16_t widths[]);
    void getRX(uint16_t widths[]);
protected:
    PICInterface();    
private:
    virtual ~PICInterface();
    static PICInterface* p_instance_;

};

#endif	/* PICINTERFACE_H */

