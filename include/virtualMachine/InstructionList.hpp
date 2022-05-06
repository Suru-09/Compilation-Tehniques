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
    InstructionList();
    // INSERT METHODS
    void insert_instr_after(const Instruction& after, const Instruction& i);
    void insert_instr(const Instruction& i);

    // Create and insert instruction methods
    void create_insert_instr_after(const Instruction& after, const int& op_code);
    void create_insert_instr(const int& op_code);

    void delete_instr_after(const Instruction& start);
};

#endif