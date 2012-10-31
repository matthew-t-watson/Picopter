/*
 * File:   I2CInterface.cpp
 * Author: Matthew
 *
 * Created on 21 October 2012, 10:38
 */

#include "I2CInterface.h"

//I2CInterface* I2CInterface::p_instance_ = NULL; //Initialise pointer
//
//I2CInterface* I2CInterface::instance()
//{
//    if (!p_instance_) //Only one instance can exist at a time
//        p_instance_ = new I2CInterface;
//}
int file_;
char filename_[20] = "/dev/i2c-1";

I2CInterface::I2CInterface()
{
    std::cout << "Constructing I2CInterface" << std::endl;
    //openInterface(); //Segfault
}

I2CInterface::~I2CInterface()
{
    std::cout << "Destructing I2CInterface" << std::endl;
    close(file_);
}

void I2CInterface::openInterface()
{
    if ((file_ = open(filename_, O_RDWR)) < 0)
    {
        std::cout << "Failed to open the i2c bus at path " << filename_ << std::endl;
    }
}

bool I2CInterface::writeRegister(unsigned char address, unsigned char registerAddress, unsigned char buf[], unsigned char len)
{
    setSlaveAddress_(address);
    i2c_smbus_write_block_data(file_, registerAddress, len, buf);
}

bool I2CInterface::readRegister(unsigned char slaveAddress, unsigned char registerAddress, unsigned char* buf, unsigned char len)
{
    setSlaveAddress_(slaveAddress);
    i2c_smbus_read_i2c_block_data(file_, registerAddress, len, buf);
}

void I2CInterface::setSlaveAddress_(unsigned char address)
{
    if (ioctl(file_, I2C_SLAVE, address) < 0)
    {
        std::cout << "Failed to set I2C slave address at file descriptor " << file_ << " errno " << errno << std::endl;
    }
}

