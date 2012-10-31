/* 
 * File:   AHRS.h
 * Author: matt
 *
 * Created on 29 October 2012, 22:38
 */

#ifndef AHRS_H
#define	AHRS_H

#include <iostream>
#include <stdint.h>

#include "MPU6050.h"


extern s_rawData rawData;

struct s_calibratedData
{
    float x;
    float y;
    float z;
    float temp;
    float p;
    float q;
    float r;
};
extern s_calibratedData calibratedData;

class AHRS
{
public:
    static AHRS* Instance();
    AHRS(const AHRS& orig);
    virtual ~AHRS();
    void getSensors(s_rawData* rawData);
    void calibrateData(s_rawData* rawData, s_calibratedData* calibratedData);
protected:
    AHRS();
private:
    static AHRS* p_instance_;
};

#endif	/* AHRS_H */

