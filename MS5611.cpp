/* 
 * File:   MS5611.cpp
 * Author: matt
 * 
 * Created on 12 February 2013, 17:01
 */

#include "MS5611.h"

MS5611Class MS5611;

MS5611Class::MS5611Class() {
    reset_();
    usleep(100000);
    getCalib_();
    lastConv_ = Pressure;
}

MS5611Class::MS5611Class(const MS5611Class& orig) {
}

MS5611Class::~MS5611Class() {
}

void MS5611Class::getPressure(int32_t *pressure) {
    static int i = 0;
    if(i == 3) { //Pressure is only updated every 4 cycles to keep conversion rate at 100Hz
	if(lastConv_ == Pressure) {
	    getRawPressure_();
	    startTempConversion_();
	    calculatePressure_(pressure);
	    lastConv_ = Temperature;
	} else if(lastConv_ == Temperature) {
	    getRawTemperature_();
	    startPressureConversion_();
	    *pressure = P_;
	    lastConv_ = Pressure;
	}
	i = 0;
    } else {
	i++;
	*pressure = P_;
    }
}

void MS5611Class::calculatePressure_(int32_t *pressure) {
    //Calculate temperature
    dT_ = D2_ - (static_cast<int32_t>(C5_) << 8); //Difference between actual and reference temperature
    TEMP_ = 2000 + ((dT_ * C6_) >> 23);
    //Calculate temperature compensated pressure
    OFF_ = (static_cast<int64_t>(C2_) << 16) + ((static_cast<int64_t>(C4_) * static_cast<int64_t>(dT_)) >> 7);
    SENS_ = ((static_cast<int64_t>(C1_)) << 15) + ((static_cast<int64_t>(C3_) * static_cast<int64_t>(dT_)) >> 8);
    P_ = (((D1_ * SENS_) >> 21) - OFF_) >> 15;
    *pressure = P_;
}

void MS5611Class::getRawPressure_() {
    I2CInterface.readRegister(MS5611_ADDRESS, 0x00, buf_, 3);
    D1_ = (buf_[0] << 16) | (buf_[1] << 8) | (buf_[2]);
}

void MS5611Class::getRawTemperature_() {
    I2CInterface.readRegister(MS5611_ADDRESS, 0x00, buf_, 3);
    D2_ = (buf_[0] << 16) | (buf_[1] << 8) | (buf_[2]);
}

void MS5611Class::startTempConversion_() {
    I2CInterface.writeRegister(MS5611_ADDRESS, D2Conv4096, 0, 0);
}

void MS5611Class::startPressureConversion_() {
    I2CInterface.writeRegister(MS5611_ADDRESS, D1Conv4096, 0, 0);
}

void MS5611Class::getCalib_() {
    I2CInterface.readRegister(MS5611_ADDRESS, C1_ADDRESS, buf_, 2);
    C1_ = (buf_[0] << 8) | buf_[1];
    I2CInterface.readRegister(MS5611_ADDRESS, C2_ADDRESS, buf_, 2);
    C2_ = (buf_[0] << 8) | buf_[1];
    I2CInterface.readRegister(MS5611_ADDRESS, C3_ADDRESS, buf_, 2);
    C3_ = (buf_[0] << 8) | buf_[1];
    I2CInterface.readRegister(MS5611_ADDRESS, C4_ADDRESS, buf_, 2);
    C4_ = (buf_[0] << 8) | buf_[1];
    I2CInterface.readRegister(MS5611_ADDRESS, C5_ADDRESS, buf_, 2);
    C5_ = (buf_[0] << 8) | buf_[1];
    I2CInterface.readRegister(MS5611_ADDRESS, C6_ADDRESS, buf_, 2);
    C6_ = (buf_[0] << 8) | buf_[1];
    I2CInterface.readRegister(MS5611_ADDRESS, CRC_ADDRESS, buf_, 2);
    CRC_ = (buf_[0] << 8) | buf_[1];
    //    std::cout << C1_ << ", " << C2_ << ", " << C3_ << ", " << C4_ << ", " << C5_ << ", " << C6_ << ", " << CRC_ << std::endl;
}

void MS5611Class::reset_() {
    I2CInterface.writeRegister(MS5611_ADDRESS, resetCommand, 0, 0);
}