#include "Instruction.hpp"

// Instruction::Instruction(const int& op_code, const std::vector<long>& args) 
// : op_code(op_code),
// args(args),
// class_name("Instruction"),
// logger(Logger{class_name})
// {}

Instruction::Instruction()
: op_code(0),
class_name("Instruction"),
logger(Logger{class_name})
{}

Instruction::Instruction(const int& op_code)
: op_code(op_code),
class_name("Instruction"),
logger(Logger{class_name})
{}

std::ostream& operator<<(std::ostream& os, const Instruction& i) {
    std::string s = "";
    for(const auto& x: i.args) {
        s += " " + std::string{Instruction::variant_to_type(x)};
    }
    os << "Instruction code: [" << i.op_code << s;
    return os;
}

std::string Instruction::variant_to_type(std::variant<long, double, void *> x) {
    return "";
}

void Instruction::set_args(const std::vector<std::variant<long, double, void *>>& arr) {
    for ( const auto& x: arr) {
        try {
            args.push_back(std::get<long> (x));
        }
        catch(const std::exception &e) {
            try {
                args.push_back(std::get<double> (x));
            }
            catch(const std::exception &e) {
                try {
                    args.push_back(std::get<void *> (x));
                }
                catch(const std::exception &e) {
                    std::cout << logger << " [SET_ARGS] Didn't find any type to match with defined VARIANT!\n";
                    exit(2);
                }
            }
        }
    }
}
