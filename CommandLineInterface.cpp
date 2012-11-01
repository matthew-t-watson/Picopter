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
        switch (lineMap_[line])
        {
            case en_stringNotDefined:
                break;
            case en_stringvalue1:
                std::cout << "First string input" << std::endl;
                break;
            case en_stringvalue2:
                std::cout << "Second string input" << std::endl;
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
    lineMap_["First input"] = en_stringvalue1;
    lineMap_["Second input"] = en_stringvalue2;
    lineMap_["exit"] = en_exit;
}
