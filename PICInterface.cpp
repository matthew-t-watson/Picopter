/* 
 * File:   PICInterface.cpp
 * Author: matt
 * 
 * Created on 28 October 2012, 12:53
 */

#include "PICInterface.h"

PICInterfaceClass PICInterface;

PICInterfaceClass::PICInterfaceClass()
{

}

PICInterfaceClass::~PICInterfaceClass()
{

}

void PICInterfaceClass::setPWM(uint16_t widths[])
{
    uint8_t widthsChar[sizeof (widths)*2];
    for (int x = 0; x < sizeof (widths); x++)
    {
	widthsChar[x * 2] = static_cast<uint8_t> (0xFF & (widths[x] >> 8));
	widthsChar[(x * 2) + 1] = static_cast<uint8_t> (0xFF & widths[x]);
    }
    I2CInterface.writeRegister(PIC_ADDRESS, REG_PWM1H, widthsChar, sizeof (widthsChar));
    unsigned char temp = 0x01;
    I2CInterface.writeRegister(PIC_ADDRESS, REG_PWMFIRE, &temp, 1);
}

void PICInterfaceClass::getRX()
{
    uint8_t widthsChar[12] = {0};
    
    //Appears to generate read errors above ~5 bytes, returning only 1's
    //	I2CInterface.readRegister(PIC_ADDRESS, REG_RX1H, widthsChar, sizeof(widthsChar));
    I2CInterface.readRegister(PIC_ADDRESS, REG_RX1H, &widthsChar[0], 2);
    I2CInterface.readRegister(PIC_ADDRESS, REG_RX2H, &widthsChar[2], 2);
    I2CInterface.readRegister(PIC_ADDRESS, REG_RX3H, &widthsChar[4], 2);
    I2CInterface.readRegister(PIC_ADDRESS, REG_RX4H, &widthsChar[6], 2);
    I2CInterface.readRegister(PIC_ADDRESS, REG_RX5H, &widthsChar[8], 2);
    I2CInterface.readRegister(PIC_ADDRESS, REG_RX6H, &widthsChar[10], 2);
    rxWidths.roll = make16_(widthsChar[0], widthsChar[1]);
    rxWidths.pitch = make16_(widthsChar[2], widthsChar[3]);
    rxWidths.throttle = make16_(widthsChar[4], widthsChar[5]);
    rxWidths.yaw = make16_(widthsChar[6], widthsChar[7]);
    rxWidths.sw1 = make16_(widthsChar[8], widthsChar[9]);
    rxWidths.sw2 = make16_(widthsChar[10], widthsChar[11]);

}

inline uint16_t PICInterfaceClass::make16_(uint8_t H, uint8_t L)
{
    return ((static_cast<uint16_t> (H) << 8) | static_cast<uint16_t> (L));
}