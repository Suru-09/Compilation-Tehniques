#ifndef LOGGER_HPP
#define LOGGER_HPP

#include<iostream>

class Logger {
private:
    std::string class_name{""};

public:
    Logger(const std::string& class_name);
    Logger() = default;
    Logger& operator=(const Logger& logger);

    friend std::ostream& operator<<(std::ostream &os, const Logger& logger);
};

#endif