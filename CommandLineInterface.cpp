/* 
 * File:   CommandLineInterface.cpp
 * Author: matt
 * 
 * Created on 01 November 2012, 16:14
 */

#include "CommandLineInterface.h"

CLI_class CLI;

CLI_class::CLI_class()
{
    initialiseMap_();
}

CLI_class::CLI_class(const CLI_class& orig)
{
}

CLI_class::~CLI_class()
{
}

void CLI_class::open()
{
    for (std::string line; std::cout << "Picopter > " && std::getline(std::cin, line);)
    {
        std::stringstream stream(line);
        int i = 0;
        while (std::getline(stream, stringbuf_[i], ' '))
        {
            i++;
        }
        
        switch (lineMap_[stringbuf_[0]])
        {
            case en_stringNotDefined:
                std::cout << "Invalid command" << std::endl;
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
            case en_exit:
                exit(1);
                break;

            default:
                std::cout << "Invalid command" << std::endl;
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
    lineMap_["exit"] = en_exit;
}
