/* 
 * File:   CommandLineInterface.h
 * Author: matt
 *
 * Created on 01 November 2012, 16:14
 */

#ifndef COMMANDLINEINTERFACE_H
#define	COMMANDLINEINTERFACE_H

#include <cstdlib>
#include <iostream>
#include <map>
#include <pthread.h>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sstream>

#include "Logger.h"
#include "Timer.h"
#include "Control.h"


enum lineString
{
    en_stringNotDefined,
    en_openlog,
    en_dumporientation,
    en_dumpsensors,
    en_dumprawsensors,
    en_dumprawmag,
    en_dumprawrx,
    en_dumprx,
    en_resetmpu,
    en_readregister,
    en_setRatePID,
    en_getRatePID,
    en_setAttitudePID,
    en_getAttitudePID,
    en_setYawPID,
    en_getYawPID,
    en_getdt,
    en_startMotorTest,
    en_exit
};

class CLI_class
{
public:
    CLI_class();
    CLI_class(const CLI_class& orig);
    virtual ~CLI_class();
    void open();
private:
    std::map<std::string, lineString> lineMap_;
    void initialiseMap_();
    std::string stringbuf_[6];
};
extern CLI_class CLI;

#endif	/* COMMANDLINEINTERFACE_H */

