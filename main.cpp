/*
 * File:   main.cpp
 * Author: matt
 *
 * Created on 21 October 2012, 22:26
 */

#include <iostream>

#include "main.h"
#include "CommandLineInterface.h"
#include "Timer.h"

using namespace std;

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
    if (!MPU6050Interface.checkConnection()) exit(1);

    //Create CLI thread + open
    pthread_t CLIthread;
    pthread_create(&CLIthread, NULL, thunk<CLI_class, &CLI_class::open>, &CLI);

    

    while (1)
    {
    }
    return 0;
}


