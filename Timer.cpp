/* 
 * File:   Timer.cpp
 * Author: matt
 * 
 * Created on 05 November 2012, 10:19
 */

#include "Timer.h"
#include "PICInterface.h"

TimerClass Timer;
pthread_mutex_t TimerMutex_;

TimerClass::TimerClass()
{
    /* Intialize sigaction struct */
    signalAction.sa_handler = &sig_handler_;

    /* Initialize timer expiration value */
    timeValue_.tv_sec = 0;
    timeValue_.tv_nsec = 2500000;

    /* Initialize timer period */
    timeInterval_.tv_sec = 0;
    timeInterval_.tv_nsec = 0;

    /* Set the time to be set value */
    timeToSet_.it_value = timeValue_;
    timeToSet_.it_interval = timeInterval_;

    /* Connect a signal handler routine to the SIGALRM signal */
    sigaction(SIGALRM, &signalAction, NULL);

    /* Allocate a timer */
    timer_create(CLOCK_REALTIME, NULL, &timerId);
}

TimerClass::TimerClass(const TimerClass& orig)
{
}

TimerClass::~TimerClass()
{
    started = false;
}

void TimerClass::start()
{
    timer_settime(timerId, 0, &timeToSet_, NULL);
    started == true;
}

void TimerClass::sig_handler_(int signum)
{
    pthread_mutex_lock(&TimerMutex_);
    uint16_t widths[] = {10000, 10000, 10000, 10000, 10000, 10000};
    PICInterface.setPWM(widths);
    PICInterface.getRX();

    static timespec oldtime;
    static timespec time;
    static timespec iterationtime;
    oldtime = time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    Timer.dt = ((time.tv_sec * 1000000000 + time.tv_nsec) - (oldtime.tv_sec * 1000000000 + oldtime.tv_nsec)) / 1000000000.0;

    AHRS.update();
    Log.precision(3);
    Log.setf(std::fstream::fixed, std::fstream::floatfield);
    Log	    << Timer.dt * 1000 << ", "
	    << AHRS.calibratedData.x << ", "
	    << AHRS.calibratedData.y << ", "
	    << AHRS.calibratedData.z << ", "
	    << AHRS.calibratedData.p << ", "
	    << AHRS.calibratedData.q << ", "
	    << AHRS.calibratedData.r << ", "
	    << AHRS.calibratedData.temp << ", "
	    << AHRS.accelAngles.phi << ", "
	    << AHRS.accelAngles.psi << ", "
	    << AHRS.orientation.phi << ", "
	    << AHRS.orientation.psi << ", "
	    << AHRS.orientation.theta << std::endl;
    Log.flush();

    //Timer aims to get as close to 400Hz as possible, mostly limited by the I2C bandwidth
    clock_gettime(CLOCK_MONOTONIC, &iterationtime);
    int inttime = 2500000 - ((iterationtime.tv_sec * 1000000000 + iterationtime.tv_nsec) - (time.tv_sec * 1000000000 + time.tv_nsec));
    if (inttime < 0)
	Timer.timeValue_.tv_nsec = 1;
    else
	Timer.timeValue_.tv_nsec = inttime;
    Timer.timeToSet_.it_value = Timer.timeValue_;
    Timer.start();
    pthread_mutex_unlock(&TimerMutex_);
}
