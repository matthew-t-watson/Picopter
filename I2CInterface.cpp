/*
 * File:   I2CInterface.cpp
 * Author: Matthew
 *
 * Created on 21 October 2012, 10:38
 */

#include "I2CInterface.h"

I2CInterfaceClass I2CInterface;

I2CInterfaceClass::I2CInterfaceClass()
{
    strcpy(filename_, "/dev/i2c-1");
    openInterface();
}

I2CInterfaceClass::~I2CInterfaceClass()
{
    close(file_);
}

void I2CInterfaceClass::openInterface()
{
    pthread_mutex_lock (&I2Cmutex_);
    if ((file_ = open(filename_, O_RDWR)) < 0)
    {
        std::cout << "Failed to open the i2c bus at path " << filename_ << std::endl;
    }
    pthread_mutex_unlock (&I2Cmutex_);
}


bool I2CInterfaceClass::writeRegister(unsigned char slaveAddress, unsigned char registerAddress, unsigned char* buf, unsigned char len)
{
    pthread_mutex_lock (&I2Cmutex_);
    setSlaveAddress_(slaveAddress);
    if (i2c_smbus_write_i2c_block_data(file_, registerAddress, len, buf) < 0)
    {
        std::cout << "I2C write failed" << std::endl;
    }
    pthread_mutex_unlock (&I2Cmutex_);
}

bool I2CInterfaceClass::readRegister(unsigned char slaveAddress, unsigned char registerAddress, unsigned char* buf, unsigned char len)
{
    pthread_mutex_lock (&I2Cmutex_);
    setSlaveAddress_(slaveAddress);
    if (i2c_smbus_read_i2c_block_data(file_, registerAddress, len, buf) != len)
    {
        std::cout << "Incorrect number of bytes read" << std::endl;
    }
    pthread_mutex_unlock (&I2Cmutex_);
}

void I2CInterfaceClass::setSlaveAddress_(unsigned char address)
{
    if (ioctl(file_, I2C_SLAVE, address) < 0)
    {
        std::cout << "Failed to set I2C slave address at file descriptor " << file_ << " errno " << errno << std::endl;
    }
}

