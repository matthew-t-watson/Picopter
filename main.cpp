/*
 * File:   main.cpp
 * Author: matt
 *
 * Created on 21 October 2012, 22:26
 */

#include <iostream>

#include "main.h"

using namespace std;

/*
 *
 */
int main(int argc, char** argv) {
    unsigned char buf[] = {0x01};
    I2CInterface::instance()->send(0x68, buf, 1);
    //MPU6050::instance()->checkConnection();
    return 0;
}

