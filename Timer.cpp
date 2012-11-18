/* 
 * File:   Timer.cpp
 * Author: matt
 * 
 * Created on 05 November 2012, 10:19
 */

#include "Timer.h"
#include "PICInterface.h"
#include "Control.h"

#define PERIOD 10000000

TimerClass Timer;
pthread_mutex_t TimerMutex_;

TimerClass::TimerClass()
{
    /* Intialize sigaction struct */
    signalAction.sa_handler = &sig_handler_;

    /* Initialize timer expiration value */
    timeValue_.tv_sec = 0;
    timeValue_.tv_nsec = PERIOD;

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
    
    started = false;
}

TimerClass::TimerClass(const TimerClass& orig)
{
}

TimerClass::~TimerClass()
{
}

void TimerClass::start()
{
    started = true;
    timer_settime(timerId, 0, &timeToSet_, NULL);
}

void TimerClass::stop()
{
    Timer.timeValue_.tv_sec = 0;
    Timer.timeValue_.tv_nsec = 0;
    Timer.timeToSet_.it_value = Timer.timeValue_;
    timer_settime(timerId, 0, &timeToSet_, NULL);
    started = false;
}

inline void TimerClass::calcdt_()
{
    oldtime_ = time_;
    clock_gettime(CLOCK_MONOTONIC, &time_);
    Timer.dt = ((time_.tv_sec * 1000000000 + time_.tv_nsec) - (oldtime_.tv_sec * 1000000000 + oldtime_.tv_nsec)) / 1000000000.0;
}

inline void TimerClass::compensate_()
{
    //Timer aims to get as close to 400Hz as possible, mostly limited by the I2C bandwidth
    clock_gettime(CLOCK_MONOTONIC, &iterationtime_);
    long inttime = PERIOD - ((iterationtime_.tv_sec * 1000000000 + iterationtime_.tv_nsec) - (time_.tv_sec * 1000000000 + time_.tv_nsec));
    if (inttime < 0)
	Timer.timeValue_.tv_nsec = 1;
    else
	Timer.timeValue_.tv_nsec = inttime;
    Timer.timeToSet_.it_value = Timer.timeValue_;
}

void TimerClass::sig_handler_(int signum)
{
    pthread_mutex_lock(&TimerMutex_);
    Timer.calcdt_();
    PICInterface.getRX();
    AHRS.update();
    Control.update();
    if (LogMan.logging)
    {
	Log << Timer.dt * 1000 << ", "
		<< AHRS.calibratedData.x << ", "
		<< AHRS.calibratedData.y << ", "
		<< AHRS.calibratedData.z << ", "
		<< AHRS.calibratedData.p << ", "
		<< AHRS.calibratedData.q << ", "
		<< AHRS.calibratedData.r << ", "
		<< AHRS.calibratedData.temp << ", "
		<< AHRS.calibratedData.mag_x << ", "
		<< AHRS.calibratedData.mag_y << ", "
		<< AHRS.calibratedData.mag_z << ", "
		<< AHRS.accelAngles.phi << ", "
		<< AHRS.accelAngles.psi << ", "
		<< AHRS.orientation.phi << ", "
		<< AHRS.orientation.psi << ", "
		<< AHRS.orientation.theta << ", "
		<< PICInterface.rx.pitch << ", "
		<< PICInterface.rx.roll << ", "
		<< PICInterface.rx.throttle << ", "
		<< PICInterface.rx.yaw << ", "
		<< PICInterface.rx.sw1 << ", "
		<< PICInterface.rx.sw2 << ", "
		<< std::endl;
    }

    //    std::cout << std::dec << AHRS.rawData_.mag_x << ", " << AHRS.rawData_.mag_y << ", " << AHRS.rawData_.mag_z << std::endl;
    //    std::cout << PICInterface.pwmwidths.frontright << ", " << PICInterface.pwmwidths.rearright << ", " << PICInterface.pwmwidths.rearleft << ", " << PICInterface.pwmwidths.frontleft << std::endl;
    //    std::cout << PICInterface.rx.pitch << ", " << PICInterface.rx.roll << ", " << PICInterface.rx.throttle << ", " << PICInterface.rx.yaw << ", " << PICInterface.rx.sw1 << ", " << PICInterface.rx.sw2 << std::endl;
    //    std::cout << PICInterface.rxWidths.pitch << ", " << PICInterface.rxWidths.roll << ", " << PICInterface.rxWidths.throttle << ", " << PICInterface.rxWidths.yaw << ", " << PICInterface.rxWidths.sw1 << ", " << PICInterface.rxWidths.sw2 << ", " << std::endl;
    //    std::cout << AHRS.orientation.phi << ", " << AHRS.orientation.psi << ", " << AHRS.orientation.theta << std::endl;
    //    std::cout << AHRS.rawData_.x << ", " << AHRS.rawData_.y << ", " << AHRS.rawData_.z << ", " << AHRS.rawData_.p << ", " << AHRS.rawData_.q << ", " << AHRS.rawData_.r << std::endl;
    //    std::cout << AHRS.calibratedData.x << ", " << AHRS.calibratedData.y << ", " << AHRS.calibratedData.z << ", " << AHRS.calibratedData.p << ", " << AHRS.calibratedData.q << ", " << AHRS.calibratedData.r << std::endl;

    Timer.compensate_();
    Timer.start();
    pthread_mutex_unlock(&TimerMutex_);
}
