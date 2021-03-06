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
: type(Type{}),
is_left_value(false),
is_constant_value(false),
class_name("ReturnValue")
{
    logger = Logger{class_name};
}

bool ReturnValue::set_constant_value(const long& val) {
    constant_value = val;
    return true;
}

bool ReturnValue::set_constant_value(const double& val) {
    constant_value = val;
    return true;
}

bool ReturnValue::set_constant_value(const std::string& val) {
    constant_value = val;
    return true;
}

ReturnValue& ReturnValue::operator=(const ReturnValue& ret_val) {
    if (this != &ret_val) {
        is_left_value = ret_val.is_left_value;
        is_constant_value = ret_val.is_constant_value;
        type = ret_val.type;
        symbol_name = ret_val.symbol_name;
        if(ret_val.is_constant_value) {
            try {
                auto x = std::get<std::string> (ret_val.constant_value);
                set_constant_value(x);
            }
            catch (const std::exception& exception) {
                try {
                    auto x = std::get<long> (ret_val.constant_value);
                    set_constant_value(x);
                }
                catch(const std::exception& exception) {
                    try {
                        auto x = std::get<double> (ret_val.constant_value);
                        set_constant_value(x);
                    }
                    catch(const std::exception& exception) {
                        std::cout << logger << "[ COPY ASSISGMENT OPERATOR FAILED ]\n";
                    }
                }
            }
        }

    }
    return *this;
}
