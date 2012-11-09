/* 
 * File:   Timer.h
 * Author: matt
 *
 * Created on 05 November 2012, 10:19
 */

#ifndef TIMER_H
#define	TIMER_H

#include <signal.h>
#include <sys/time.h>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>

#include "MPU6050.h"
#include "AHRS.h"
#include "Logger.h"

class TimerClass
{
public:
    TimerClass();
    TimerClass(const TimerClass& orig);
    virtual ~TimerClass();
    
    void start();
    float dt;
    timer_t timerId; /* id  for the posix timer */
    struct sigaction signalAction; /* signal action handler struct */
    bool started;
private:
    static void sig_handler_(int signum);
    struct itimerspec timeToSet_; /* time to be set */
    struct timespec timeValue_; /* timer expiration value */
    struct timespec timeInterval_; /* timer period */

};

extern TimerClass Timer;
#endif	/* TIMER_H */

