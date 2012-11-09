/* 
 * File:   Timer.cpp
 * Author: matt
 * 
 * Created on 05 November 2012, 10:19
 */

#include "Timer.h"
#include "PICInterface.h"

TimerClass Timer;

TimerClass::TimerClass()
{
    /* Intialize sigaction struct */
    signalAction.sa_handler = &sig_handler_;

    /* Initialize timer expiration value */
    timeValue.tv_sec = 0;
    timeValue.tv_nsec = 2500000;

    /* Initialize timer period */
    timeInterval.tv_sec = 0;
    timeInterval.tv_nsec = 0;

    /* Set the time to be set value */
    timeToSet.it_value = timeValue;
    timeToSet.it_interval = timeInterval;

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
}

void TimerClass::start()
{
    timer_settime(timerId, 0, &timeToSet, NULL);
}

void TimerClass::sig_handler_(int signum)
{
    //uint16_t widths[] = {100,200,400,800,1600,3200};
    //PICInterface.setPWM(widths);

    static timespec oldtime;
    static timespec time;
    static timespec iterationtime;
    oldtime = time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    Timer.dt = ((time.tv_sec * 1000000000 + time.tv_nsec) - (oldtime.tv_sec * 1000000000 + oldtime.tv_nsec)) / 1000000000.0;

    AHRS.update();
    Log.precision(3);
    Log.setf(std::fstream::fixed, std::fstream::floatfield);
    Log << Timer.dt * 1000 << ", "
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
    //Log.flush();

    
    
//	    Log << AHRS.rawData_.p << ", "
//	    << AHRS.rawData_.q << ", "
//	    << AHRS.rawData_.r << ", " <<std::endl;
    
    //std::cout << signum << " " << Timer.dt << " " << timer_getoverrun(&Timer.timerId) << std::endl;

    //    clock_gettime(CLOCK_MONOTONIC, &iterationtime);
    //    Timer.timeValue.tv_nsec = 2500000 - ((iterationtime.tv_sec*1000000000 + iterationtime.tv_nsec) - (time.tv_sec*1000000000 + time.tv_nsec));
    //    Timer.timeToSet.it_value = Timer.timeValue;
    //std::cout << AHRS.orientation.phi << ", " << AHRS.orientation.psi << ", " << AHRS.orientation.theta << std::endl;
    Timer.start();
}
