#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "Logger.hpp"

#include <iostream>
#include <variant>
#include <vector>

class Instruction {
public:
    enum InstructionType {
        O_HALT = 0, 
        O_ADD_C = 1, O_ADD_D = 2, O_ADD_I = 3,
        O_AND_C = 4, O_AND_D = 5, O_AND_I = 6,
        O_CALL = 7, O_CALLEXT = 8, O_CAST_C_D = 9,
        O_CAST_C_I = 10, O_CAST_D_C = 11, O_CAST_D_I = 12,
        O_CAST_I_C = 13, O_CAST_I_D = 14, O_DIV_C = 15,
        O_DIV_D = 16, O_DIV_I = 17, O_DROP = 18, O_ENTER = 19,
        O_EQ_A = 20, O_EQ_C = 21, O_EQ_D = 22, O_EQ_I = 23,
        O_GREATER_C = 24, O_GREATER_D = 25, O_GREATER_I = 26,
        O_GREATEREQ_C = 27, O_GREATEREQ_D = 28, O_GREATEREQ_I = 29,
        O_INSERT = 30, O_JF_A = 31, O_JF_C = 32, O_JF_D = 33,
        O_JF_I = 34, O_JMP = 35, O_JT_A = 36, O_JT_C = 37,
        O_JT_D = 38, O_JT_I = 39, O_LESS_C = 40, O_LESS_D = 41,
        O_LESS_I = 42, O_LESSEQ_C = 43, O_LESSEQ_D = 44,
        O_LESSEQ_I = 45, O_MUL_C = 46, O_MUL_D = 47, O_MUL_I = 48,
        O_NEG_C = 49, O_NEG_D = 50, O_NEG_I = 51, O_NOP = 52,
        O_NOT_A = 53, O_NOT_C = 54, O_NOT_D = 55, O_NOT_I = 56,
        O_NOTEQ_A = 57, O_NOTEQ_C = 58, O_NOTEQ_D = 59,
        O_NOTEQ_I = 60, O_OFFSET = 61, O_OR_A = 62, O_OR_D = 63,
        O_OR_I = 64, O_PUSHFPADDR = 65, O_PUSHCT_A = 66,
        O_PUSHCT_C = 67, O_PUSHCT_D = 68, O_PUSHCT_I = 69,
        O_RET = 70, O_STORE = 71, O_SUB_C = 72, O_SUB_D = 73,
        O_SUB_I = 74, O_LOAD = 75, O_AND_A = 76
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


    friend class VirtualMachine;
};

#endif // defined