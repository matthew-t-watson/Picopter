/* 
 * File:   Logger.cpp
 * Author: matt
 * 
 * Created on 01 November 2012, 22:49
 */

#include "Logger.h"

LoggerClass LogMan;
std::fstream Log;

LoggerClass::LoggerClass()
{
    logging = false;
}

LoggerClass::LoggerClass(const LoggerClass& orig)
{
}

LoggerClass::~LoggerClass()
{
    Log.close();
}

void LoggerClass::open(const char* filename)
{
    Log.open(filename, std::fstream::out);
//    Log.precision(3);
//    Log.setf(std::fstream::fixed, std::fstream::floatfield);
    logging = true;
//    Log.setf(std::fstream::unitbuf);
}
