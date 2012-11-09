/* 
 * File:   Control.cpp
 * Author: matt
 * 
 * Created on 08 November 2012, 16:05
 */

#include "Control.h"
#include "PICInterface.h"

ControlClass::ControlClass()
{
}

ControlClass::ControlClass(const ControlClass& orig)
{
}

ControlClass::~ControlClass()
{
}

void ControlClass::update()
{
    PICInterface.getRX();
}

//void ControlClass::getRanges