/* 
 * File:   AHRS.cpp
 * Author: matt
 * 
 * Created on 29 October 2012, 22:38
 */

#include "AHRS.h"

AHRS* AHRS::p_instance_ = NULL; //Initialise pointer
s_rawData rawData;
s_calibratedData calibratedData;

AHRS* AHRS::Instance()
{
    if(!p_instance_)
        p_instance_ = new AHRS;
}

AHRS::AHRS()
{
}

AHRS::AHRS(const AHRS& orig)
{
}

AHRS::~AHRS()
{
}

void AHRS::getSensors(s_rawData* rawData)
{
    MPU6050::instance()->getSensors(rawData);
}

void AHRS::calibrateData(s_rawData* rawData, s_calibratedData* calibratedData) //Need to add scales and zero points + any required inversions
{
    calibratedData->x = (rawData->x - 0) * 0;
    calibratedData->y = (rawData->y - 0) * 0;
    calibratedData->z = (rawData->z - 0) * 0;
    calibratedData->temp = (rawData->temp - 0) * 0;
    calibratedData->p = (rawData->p - 0) * 0;
    calibratedData->q = (rawData->q - 0) * 0;
    calibratedData->r = (rawData->r - 0) * 0;
}
        

