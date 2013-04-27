/*
 * File:   main.cpp
 * Author: matt
 *
 * Created on 21 October 2012, 22:26
 */


#include "main.h"
#include "PICInterface.h"
#include "HMC5883L.h"

int main(int argc, char** argv)
{
    MPU6050.initialise();
    HMC5883L.initialise(); 
    Timer.start();
    CLI.open();    
    while (1)
    {
        sleep(1000);
    }
}




