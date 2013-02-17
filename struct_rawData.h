/* 
 * File:   struct_rawData.h
 * Author: matt
 *
 * Created on 16 November 2012, 23:04
 */

#ifndef STRUCT_RAWDATA_H
#define	STRUCT_RAWDATA_H

#include <stdint.h>

struct s_rawData
{
    int16_t x;
    int16_t y;
    int16_t z;
    int16_t temp;
    int16_t p;
    int16_t q;
    int16_t r;
    int16_t mag_x;
    int16_t mag_y;
    int16_t mag_z;
    int32_t pressure;
};

#endif	/* STRUCT_RAWDATA_H */

