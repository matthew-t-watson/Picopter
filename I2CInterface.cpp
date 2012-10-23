/*
 * File:   I2CInterface.cpp
 * Author: Matthew
 *
 * Created on 21 October 2012, 10:38
 */

#include "I2CInterface.h"

I2CInterface* I2CInterface::p_instance_ = NULL; //Initialise pointer

I2CInterface* I2CInterface::instance()
{
    if (!p_instance_) //Only one instance can exist at a time
        p_instance_ = new I2CInterface;
}

I2CInterface::I2CInterface()
{
    initialise_();
}

I2CInterface::~I2CInterface()
{
    close(file_);
}

void I2CInterface::initialise_()
{
    char filename[] = "/dev/i2c-1";
    if ((file_ = open(filename, O_RDWR)) < 0)
    {
        /* ERROR HANDLING: you can check errno to see what went wrong */
        std::cout << "Failed to open the i2c bus" << std::endl;
    }
    std::cout << "Opened /dev/i2c-1 at file descriptor " << file_ << std::endl;
}

bool I2CInterface::send(unsigned char address, unsigned char buf[], unsigned char len)
{
    //initialise_();
    setSlaveAddress_(address);
    if (write(file_, buf, len) != len)
    {
        std::cout << "Unable to send I2C data" << std::endl;
        return false;
    }
    return true;
}

bool I2CInterface::get(unsigned char slaveAddress, unsigned char registerAddress, unsigned char* buf, unsigned short len)
{
    std::cout << "Getting from I2C, setting slave address" << std::endl;
    setSlaveAddress_(slaveAddress);
    std::cout << "Slave address set, writing" << std::endl;
    if (write(file_, &registerAddress, 1) != 1) //Fire out desired register address
    {
        std::cout << "Failed to send desired register address" << std::endl;
        return false;
    }
    std::cout << "Desired register address sent" << std::endl;
    if (read(file_, buf, len) != len) //Read in desired number of bytes. Register address will increment automagically
    {
        std::cout << "I2C bytes requested != bytes returned" << std::endl;
        return false;
    }
    return true;
}

void I2CInterface::setSlaveAddress_(unsigned char address)
{
    std::cout << "Setting slave address at file descriptor " << file_ << std::endl;
    if (ioctl(file_, I2C_SLAVE, address) < 0)
    {
        std::cout << "Failed to set I2C slave address errno " << errno << std::endl;
    }
}

