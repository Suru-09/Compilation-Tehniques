#ifndef RETURN_VALUE_HPP
#define RETURN_VALUE_HPP

#include <variant>

#include "Type.hpp"
#include "Logger.hpp"

class ReturnValue {
private:
    Type type;
    bool is_left_value;
    bool is_constant_value;
    std::variant<long, double, std::string> constant_value;
    std::string class_name;
    Logger logger;
public:
    ReturnValue();
    ReturnValue(const Type& type, const bool& is_left_value, const bool& is_constant_value = 0);
    bool set_constant_value(long val);
    bool set_constant_value(double val);
    bool set_constant_value(std::string val);
    ReturnValue& operator=(const ReturnValue& ret_val);

    friend class SyntacticAnalyzer;
};

#endif