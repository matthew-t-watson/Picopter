/* 
 * File:   DLPF.cpp
 * Author: matt
 * 
 * Created on 18 November 2012, 11:43
 */

#include "DLPF.h"

DLPF::DLPF()
{
}

DLPF::DLPF(const DLPF& orig)
{
}

DLPF::~DLPF()
{
}

void DLPF::iterate(double* tofilter, double* output)
{
    xv_[0] = xv_[1];
    xv_[1] = xv_[2];
    xv_[2] = xv_[3];
    xv_[3] = *tofilter / GAIN;
    yv_[0] = yv_[1];
    yv_[1] = yv_[2];
    yv_[2] = yv_[3];
    yv_[3] =   (xv_[0] + xv_[3]) + 3 * (xv_[1] + xv_[2]) + (  0.2780599176 * yv_[0]) + ( -1.1828932620 * yv_[1]) + (  1.7600418803 * yv_[2]);
    *output = yv_[3];
}