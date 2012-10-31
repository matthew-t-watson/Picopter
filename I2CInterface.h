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

class I2CInterface
{
public:
    I2CInterface();
    virtual ~I2CInterface();
    static bool writeRegister(unsigned char address, unsigned char registerAddress, unsigned char buf[], unsigned char len);
    static bool readRegister(unsigned char slaveAddress, unsigned char registerAddress, unsigned char* buf, unsigned char len);
    static void openInterface();
    
private:
    //static int file_;
    //static char filename_[20];
    static void setSlaveAddress_(unsigned char address);
};

#endif	/* I2CINTERFACE_H */

