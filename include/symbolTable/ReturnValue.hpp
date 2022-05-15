#ifndef RETURN_VALUE_HPP
#define RETURN_VALUE_HPP

#include <variant>

#include "Type.hpp"
#include "Logger.hpp"
#include "Symbol.hpp"

class ReturnValue {
private:
    Type type;
    bool is_left_value;
    bool is_constant_value;
    std::variant<long, double, std::string> constant_value;
    std::string class_name;
    std::string symbol_name;
    Logger logger;
    Symbol symb;
public:
    ReturnValue();
    ReturnValue(const Type& type, const bool& is_left_value, const bool& is_constant_value = 0);
    bool set_constant_value(const long& val);
    bool set_constant_value(const double& val);
    bool set_constant_value(const std::string& val);
    ReturnValue& operator=(const ReturnValue& ret_val);

    friend class SyntacticAnalyzer;
};

#endif