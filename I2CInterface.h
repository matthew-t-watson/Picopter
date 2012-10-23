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
#include <linux/i2c.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

class I2CInterface
{
public:
    static I2CInterface* instance();
    bool send(unsigned char address, unsigned char buf[], unsigned char len);
    bool get(unsigned char slaveAddress, unsigned char registerAddress, unsigned char* buf, unsigned short len);

private:
    I2CInterface();
    virtual ~I2CInterface();
    static I2CInterface* p_instance_;

    void initialise_();
    void setSlaveAddress_(unsigned char address);
    int file_;
};

#endif	/* I2CINTERFACE_H */

