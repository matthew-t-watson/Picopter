/* 
 * File:   CommandLineInterface.cpp
 * Author: matt
 * 
 * Created on 01 November 2012, 16:14
 */

#include "CommandLineInterface.h"
#include "PICInterface.h"

CLI_class CLI;
pthread_mutex_t CLImutex;

CLI_class::CLI_class()
{
    initialiseMap_();
}

CLI_class::CLI_class(const CLI_class& orig)
{
    std::string stringbuf_[6] = {0};
}

CLI_class::~CLI_class()
{
}

void CLI_class::open()
{
    while (1)
    {
	std::string line;
	do
	{
	    std::cout << "Picopter > ";
	    do
	    {
		std::cin.clear();
		line.clear();
		std::getline(std::cin, line);
	    }
	    while (std::cin.fail());
	}
	while (line.length() == 0);

	std::stringstream stream(line);
	int i = 0;
	while (std::getline(stream, stringbuf_[i], ' '))
	{
	    i++;
	}

	switch (lineMap_[stringbuf_[0]])
	{
	    case en_stringNotDefined:
		std::cout << stringbuf_[0] << " isn't a valid command" << std::endl;
		break;

	    case en_openlog:
		std::cout << "Opening log at " << stringbuf_[1] << std::endl;
		LogMan.open(stringbuf_[1].c_str());
		break;

	    case en_writelog:
		std::cout << "Writing " << stringbuf_[1] << " to log" << std::endl;
		Log << stringbuf_[1];
		break;

	    case en_starttimer:
		Timer.start();
		break;

	    case en_readconfig:
		std::cout << Config.getValueOfKey<std::string > (stringbuf_[1]) << std::endl;
		break;

	    case en_dumpsensors:
		if(!Timer.started) AHRS.update();
		std::cout << AHRS.calibratedData.x << "\t" << AHRS.calibratedData.y << "\t" << AHRS.calibratedData.z << "\t" << AHRS.calibratedData.p << "\t" << AHRS.calibratedData.q << "\t" << AHRS.calibratedData.r << "\t" << AHRS.calibratedData.temp << std::endl;
		break;

	    case en_dumprawsensors:
		if(!Timer.started) AHRS.update();
		std::cout << AHRS.rawData_.x << "\t" << AHRS.rawData_.y << "\t" << AHRS.rawData_.z << "\t" << AHRS.rawData_.p << "\t" << AHRS.rawData_.q << "\t" << AHRS.rawData_.r << std::endl;
		break;

	    case en_dumprx:
		if(!Timer.started) PICInterface.getRX();
		std::cout << PICInterface.rxWidths.pitch << ", " << PICInterface.rxWidths.roll << ", " << PICInterface.rxWidths.throttle << ", " << PICInterface.rxWidths.yaw << ", " << PICInterface.rxWidths.sw1 << ", " << PICInterface.rxWidths.sw2 << ", " << std::endl;
		break;

	    case en_resetmpu:
		MPU6050Interface.initialise();
		std::cout << "mpu reset" << std::endl;
		break;

	    case en_readregister:
		unsigned char buf[32];
		I2CInterface.readRegister(static_cast<unsigned char> (atoi(stringbuf_[1].c_str())), static_cast<unsigned char> (atoi(stringbuf_[2].c_str())), buf, static_cast<unsigned char> (atoi(stringbuf_[3].c_str())));
		std::cout << buf << std::endl;
		break;

	    case en_exit:
		exit(1);
		break;

	    default:
		std::cout << stringbuf_[0] << " isn't a valid command" << std::endl;
		break;
	}
    }
}

void CLI_class::initialiseMap_()
{
    lineMap_[""] = en_stringNotDefined;
    lineMap_["openlog"] = en_openlog;
    lineMap_["writelog"] = en_writelog;
    lineMap_["starttimer"] = en_starttimer;
    lineMap_["readconfig"] = en_readconfig;
    lineMap_["dr"] = en_dumprawsensors;
    lineMap_["ds"] = en_dumpsensors;
    lineMap_["drx"] = en_dumprx;
    lineMap_["resetmpu"] = en_resetmpu;
    lineMap_["rr"] = en_readregister;
    lineMap_["exit"] = en_exit;
}
