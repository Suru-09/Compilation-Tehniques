#include "Instruction.hpp"

Instruction::Instruction()
: op_code(-1),
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
    os << "Instruction code: [" << i.op_code  << "]" << s;
    return os;
}

std::string Instruction::variant_to_type(std::variant<long, double, void *> x) {
    try {
        std::get<long> (x);
        return "long";
    }
    catch(const std::exception &e) {
        try {
            std::get<double> (x);
            return "double";
        }
        catch(const std::exception &e) {
            try {
                std::get<void *> (x);
                return "void";
            }
            catch(const std::exception &e) {
                std::cout << " [VARIANT_TO_TYPES] Didn't find any type to match with given VARIANT!\n";
                exit(2);
            }
        }
    }
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

bool Instruction::empty() {
    return op_code == 0 ? true : false;
}
