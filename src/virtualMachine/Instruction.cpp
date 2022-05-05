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

std::ostream& operator<<(std::ostream& os, const Instruction& i) {
    std::string s = "";
    for(const auto& x: i.args) {
        s += " " + std::string{Instruction::variant_to_type(x)};
    }
    os << "Instruction code: [" << i.op_code << s << "\n";
    return os;
}

std::string Instruction::variant_to_type(std::variant<long, double, void *> x) {
    return "";
}
