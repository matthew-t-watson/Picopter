/*
 * File:   main.cpp
 * Author: matt
 *
 * Created on 21 October 2012, 22:26
 */


#include "main.h"
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
    MPU6050Interface.initialise();
    //Create CLI thread + open
//    pthread_t CLIthread;
//    pthread_create(&CLIthread, NULL, thunk<CLI_class, &CLI_class::open>, &CLI);
    Timer.start();
    CLI.open();
    while (1)
    {
        sleep(1000);
    }
}


