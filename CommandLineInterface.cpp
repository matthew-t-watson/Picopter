/* 
 * File:   CommandLineInterface.cpp
 * Author: matt
 * 
 * Created on 01 November 2012, 16:14
 */

#include "CommandLineInterface.h"

CLI_class CLI;
pthread_mutex_t CLImutex;

CLI_class::CLI_class() {
    initialiseMap_();
}

CLI_class::CLI_class(const CLI_class& orig) {
    std::string stringbuf_[6] = {0};
}

CLI_class::~CLI_class() {
}

void CLI_class::open() {
    while(1) {
	std::string line;
	do {
	    std::cout << "Picopter > ";
	    do {
		std::cin.clear();
		line.clear();
		std::getline(std::cin, line);
	    } while(std::cin.fail());
	} while(line.length() == 0);

	std::stringstream stream(line);
	int i = 0;
	while(std::getline(stream, stringbuf_[i], ' ')) {
	    i++;
	}

	switch(lineMap_[stringbuf_[0]]) {
	    case en_stringNotDefined:
		std::cout << stringbuf_[0] << " isn't a valid command" << std::endl;
		break;

	    case en_openlog:
		std::cout << "Opening log at " << stringbuf_[1] << std::endl;
		LogMan.open(stringbuf_[1].c_str());
		break;
		
	    case en_dumporientation:
		std::cout << "Pitch " << AHRS.orientation.pitch << ", roll " << AHRS.orientation.roll << ", yaw" << AHRS.orientation.yaw << std::endl;
		break;

	    case en_dumpsensors:
		if(!Timer.started) {
		    AHRS.update();
		}
		std::cout << AHRS.calibratedData.x << ", " << AHRS.calibratedData.y << ", " << AHRS.calibratedData.z << ", " << AHRS.calibratedData.p << ", " << AHRS.calibratedData.q << ", " << AHRS.calibratedData.r << ", " << AHRS.calibratedData.temp << ", " << AHRS.calibratedData.pressure << ", " << AHRS.calibratedData.altitude << std::endl;
		break;

	    case en_dumprawsensors:
		if(!Timer.started) {
		    AHRS.update();
		}
		std::cout << std::dec << AHRS.rawData_.x << ", " << AHRS.rawData_.y << ", " << AHRS.rawData_.z << ", " << AHRS.rawData_.p << ", " << AHRS.rawData_.q << ", " << AHRS.rawData_.r << std::endl;
		break;

	    case en_dumprawmag:
		if(!Timer.started) {
		    AHRS.update();
		}
		std::cout << AHRS.rawData_.mag_x << ", " << AHRS.rawData_.mag_y << ", " << AHRS.rawData_.mag_z << std::endl;
		break;

	    case en_dumprawrx:
		if(!Timer.started) {
		    PICInterface.getRX();
		}
		std::cout << std::dec << PICInterface.rxWidths.pitch << ", " << PICInterface.rxWidths.roll << ", " << PICInterface.rxWidths.throttle << ", " << PICInterface.rxWidths.yaw << ", " << PICInterface.rxWidths.sw1 << ", " << PICInterface.rxWidths.sw2 << ", " << std::endl;
		break;

	    case en_dumprx:
		if(!Timer.started) {
		    PICInterface.getRX();
		}
		if(PICInterface.rx.sw2 == false){ //Rate mode
		    std::cout << std::dec << "Pitch rate demand " << PICInterface.rx.pitchRateDem << ", roll rate demand " << PICInterface.rx.rollRateDem << ", throttle " << PICInterface.rx.throttleDem << ", yaw rate demand " << PICInterface.rx.yawRateDem << std::endl;
		}
		else{ //Attitude mode
		    std::cout << std::dec << "Pitch demand " << PICInterface.rx.pitchDem << ", roll demand " << PICInterface.rx.rollDem << ", throttle " << PICInterface.rx.throttleDem << ", yaw rate demand " << PICInterface.rx.yawRateDem << std::endl;
		}
		break;

	    case en_resetmpu:
		MPU6050.initialise();
		std::cout << "mpu reset" << std::endl;
		break;

	    case en_readregister:
		unsigned char buf[32];
		I2CInterface.readRegister(static_cast<unsigned char>(atoi(stringbuf_[1].c_str())), static_cast<unsigned char>(atoi(stringbuf_[2].c_str())), buf, static_cast<unsigned char>(atoi(stringbuf_[3].c_str())));
		std::cout << buf << std::endl;
		break;

	    case en_setRatePID:
		Control.setRatePID(atof(stringbuf_[1].c_str()), atof(stringbuf_[2].c_str()), atof(stringbuf_[3].c_str()));
		break;

	    case en_getRatePID:
		Control.getRatePID();
		break;

	    case en_setAttitudePID:
		Control.setAttitudePID(atof(stringbuf_[1].c_str()), atof(stringbuf_[2].c_str()), atof(stringbuf_[3].c_str()));
		break;

	    case en_getAttitudePID:
		Control.getAttitudePID();
		break;

	    case en_setYawPID:
		break;

	    case en_getYawPID:
		break;

	    case en_getdt:
		std::cout << Timer.dt << std::endl;
		break;

	    case en_startMotorTest:
		Control.startMotorTest();
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

void CLI_class::initialiseMap_() {
    lineMap_[""] = en_stringNotDefined;
    lineMap_["openlog"] = en_openlog;
    lineMap_["do"] = en_dumporientation;
    lineMap_["dr"] = en_dumprawsensors;
    lineMap_["ds"] = en_dumpsensors;
    lineMap_["drm"] = en_dumprawmag;
    lineMap_["drrx"] = en_dumprawrx;
    lineMap_["drx"] = en_dumprx;
    lineMap_["resetmpu"] = en_resetmpu;
    lineMap_["rr"] = en_readregister;
    lineMap_["srpid"] = en_setRatePID;
    lineMap_["grpid"] = en_getRatePID;
    lineMap_["sypid"] = en_setYawPID;
    lineMap_["gypid"] = en_getYawPID;
    lineMap_["sapid"] = en_setAttitudePID;
    lineMap_["gapid"] = en_getAttitudePID;
    lineMap_["gdt"] = en_getdt;
    lineMap_["startmotortest"] = en_startMotorTest;
    lineMap_["exit"] = en_exit;
}
