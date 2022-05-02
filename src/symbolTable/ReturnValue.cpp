#include "ReturnValue.hpp"

ReturnValue::ReturnValue(const Type& type, const bool& is_left_value, const bool& is_constant_value) 
: type(type),
is_left_value(is_left_value),
is_constant_value(is_constant_value),
class_name("ReturnValue")
{
    logger = Logger{class_name};
}

ReturnValue::ReturnValue() 
: class_name("ReturnValue")
{
    logger = Logger{class_name};
}

bool ReturnValue::set_constant_value(long val) {
    if( is_constant_value ) {
        constant_value = val;
        return true;
    }
    return false;
}

bool ReturnValue::set_constant_value(double val) {
    if( is_constant_value ) {
        constant_value = val;
        return true;
    }
    return false;
}

bool ReturnValue::set_constant_value(std::string val) {
    if( is_constant_value ) {
        constant_value = val;
        return true;
    }
    return false;
}