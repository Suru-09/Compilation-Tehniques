#ifndef INSTRUCTION_LIST_HPP
#define INSTRUCTION_LIST_HPP

#include <list>

#include "Instruction.hpp"
#include "Logger.hpp"

class InstructionList {
private:
    std::list<Instruction> instr_list;
    std::string class_name;
    Logger logger;
public:
    void insert_struction_after(const Instruction& after, const Instruction& i);
    InstructionList();
};

#endif