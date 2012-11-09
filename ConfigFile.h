/* 
 * File:   ConfigFile.h
 * Author: matt
 *
 * Created on 05 November 2012, 14:41
 */

#ifndef CONFIGFILE_H
#define	CONFIGFILE_H

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <typeinfo>

template <typename T>
static std::string T_to_string(T const &val)
{
    std::ostringstream ostr;
    ostr << val;

    return ostr.str();
}

template <typename T>
static T string_to_T(std::string const &val)
{
    std::istringstream istr(val);
    T returnVal;
    if (!(istr >> returnVal))
        std::cout << "CFG: Not a valid " << (std::string)typeid (T).name() << " received!\n" << std::endl;

    return returnVal;
}

template <>
std::string string_to_T(std::string const &val)
{
    return val;
}

class ConfigFile
{
private:
    std::map<std::string, std::string> contents;
    std::string fName;
    void removeComment(std::string &line) const;
    bool onlyWhitespace(const std::string &line) const;
    bool validLine(const std::string &line) const;
    void extractKey(std::string &key, size_t const &sepPos, const std::string &line) const;
    void extractValue(std::string &value, size_t const &sepPos, const std::string &line) const;
    void extractContents(const std::string &line);
    void parseLine(const std::string &line, size_t const lineNo);
    void ExtractKeys();
public:
    ConfigFile(const std::string &fName);
    bool keyExists(const std::string &key) const;
    template <typename ValueType>
    ValueType getValueOfKey(const std::string &key, ValueType const &defaultValue = ValueType()) const;


};

template <typename ValueType>
ValueType ConfigFile::getValueOfKey(const std::string &key, ValueType const &defaultValue) const
{
    if (!keyExists(key))
        return defaultValue;

    return string_to_T<ValueType > (contents.find(key)->second);
}

extern ConfigFile Config;

#endif	/* CONFIGFILE_H */

