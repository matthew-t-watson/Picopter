/*
 * File:   main.cpp
 * Author: matt
 *
 * Created on 21 October 2012, 22:26
 */


#include "main.h"
#include "PICInterface.h"
#include "HMC5883L.h"
/*
 *
 */
template <class T, void(T::*member_function)()>
void* thunk(void* p)
{
    (static_cast<T*> (p)->*member_function)();
    return 0;
}

int main(int argc, char** argv)
{
    HMC5883L.initialise();
    MPU6050.initialise();
//    pthread_t CLIthread;
//    pthread_create(&CLIthread, NULL, thunk<CLI_class, &CLI_class::open>, &CLI);    
//    Timer.start();
    CLI.open();
    
    while (1)
    {
        sleep(1000);
    }
}




