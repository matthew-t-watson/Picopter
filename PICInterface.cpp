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
    uint8_t widthsChar[sizeof(widths)*2];
    for (int x = 0; x < sizeof(widths); x++)
    {
        widthsChar[x*2] = static_cast<uint8_t> (0xFF & (widths[x] >> 8));
        widthsChar[(x*2)+1] = static_cast<uint8_t> (0xFF & widths[x]);
    }
    I2CInterface.writeRegister(PIC_ADDRESS, REG_PWM1H, widthsChar, sizeof(widthsChar));
}

void PICInterfaceClass::getRX(uint16_t widths[])
{
    uint8_t widthsChar[NUMBER_RX_CHANNELS * 2];
    I2CInterface.readRegister(PIC_ADDRESS, REG_RX1H, widthsChar, NUMBER_RX_CHANNELS * 2);
    for (int x = 0; x < NUMBER_RX_CHANNELS; x++)
    {
        widths[x] = static_cast<uint16_t> ((widthsChar[x * 2] << 8) | widthsChar[(x * 2) + 1]);
    }
}