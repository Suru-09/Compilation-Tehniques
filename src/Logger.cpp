#include "Logger.hpp"

std::ostream& operator<<(std::ostream &os, const Logger& logger) {
    os << "[" << logger.class_name << "]  ";
    return os;
}

Logger::Logger(const std::string& class_name)
: class_name(class_name)
{}

Logger& Logger::operator=(const Logger& logger) {
    class_name = logger.class_name;
    return *this;
}
