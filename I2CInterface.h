/*
 * File:   I2CInterface.h
 * Author: Matthew
 *
 * Created on 21 October 2012, 10:38
 */

#ifndef I2CINTERFACE_H
#define	I2CINTERFACE_H


#include <iostream>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
//#include <linux/i2c.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

class I2CInterfaceClass
{
public:
    I2CInterfaceClass();
    virtual ~I2CInterfaceClass();
    bool writeRegister(unsigned char address, unsigned char registerAddress, unsigned char buf[], unsigned char len);
    bool readRegister(unsigned char slaveAddress, unsigned char registerAddress, unsigned char* buf, unsigned char len);
    void openInterface();

private:
    void setSlaveAddress_(unsigned char address);
    int file_;
    char filename_[20];
};
extern I2CInterfaceClass I2CInterface;

#endif	/* I2CINTERFACE_H */

