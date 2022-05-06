#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "Logger.hpp"

#include <iostream>
#include <variant>
#include <vector>

class Instruction {
private:
    enum InstructionType {
        O_ADD_C = 0, O_ADD_D = 1, O_ADD_I = 2,
    };
    int op_code;
    std::vector<std::variant<long, double, void *>> args;
    std::string class_name;
    Logger logger;

public:
    Instruction();
    Instruction(const int& op_code);
    friend std::ostream& operator<<(std::ostream& os, const Instruction& i);
    static std::string variant_to_type(std::variant<long, double, void *> x);
    void set_args(const std::vector<std::variant<long, double, void *>>& arr);

    inline bool operator==(const Instruction& i) { // TO DO: Might need reimplementaion
        return (this->op_code == i.op_code && this->args == i.args);
    }
};

#endif // defined