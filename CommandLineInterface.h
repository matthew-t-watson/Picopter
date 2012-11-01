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


enum lineString
{
    en_stringNotDefined,
    en_stringvalue1,
    en_stringvalue2,
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
};
extern CLI_class CLI;

#endif	/* COMMANDLINEINTERFACE_H */

