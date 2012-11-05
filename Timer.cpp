/* 
 * File:   Timer.cpp
 * Author: matt
 * 
 * Created on 05 November 2012, 10:19
 */

#include "Timer.h"

TimerClass Timer;

TimerClass::TimerClass()
{
}

TimerClass::TimerClass(const TimerClass& orig)
{
}

TimerClass::~TimerClass()
{
}

void TimerClass::start()
{
    timer_t timerId; /* id  for the posix timer */
    struct itimerspec timeToSet; /* time to be set */
    struct timespec timeValue; /* timer expiration value */
    struct timespec timeInterval; /* timer period */
    struct sigaction signalAction; /* signal action handler struct */

    /* Intialize sigaction struct */
    signalAction.sa_handler = &sig_handler_;

    /* Initialize timer expiration value */
    timeValue.tv_sec = 0;
    timeValue.tv_nsec = 2500000;

    /* Initialize timer period */
    timeInterval.tv_sec = 0;
    timeInterval.tv_nsec = 2500000;

    /* Set the time to be set value */
    timeToSet.it_value = timeValue;
    timeToSet.it_interval = timeInterval;

    /* Connect a signal handler routine to the SIGALRM siganl */
    sigaction(SIGALRM, &signalAction, NULL);

    /* Allocate a timer */
    timer_create(CLOCK_REALTIME, NULL, &timerId);

    timer_settime(timerId, 0, &timeToSet, NULL);
}

void TimerClass::sig_handler_(int signum)
{
    static timespec oldtime;
    static timespec time;
    oldtime = time;
    clock_gettime (CLOCK_REALTIME, &time);
    MPU6050Interface.getSensors(&rawData);
    Log << (time.tv_nsec - oldtime.tv_nsec)/1000000.0 << "\t\t" << rawData.x << "\t" << rawData.y << "\t" << rawData.z << std::endl;
}
