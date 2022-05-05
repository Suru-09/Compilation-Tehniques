#include "InstructionList.hpp"

InstructionList::InstructionList()
: class_name("InstructionList"),
logger(Logger{class_name})
{}

void InstructionList::insert_struction_after(const Instruction& after, const Instruction& i) {
    auto it = std::find(instr_list.begin(), instr_list.end(), after);
    if( it != instr_list.end() ) {
        instr_list.insert(it, i);
    }
    else {
        std::cout << logger << "Element: [" << i << "] NOT FOUND in INSTRUCTION_LIST!\n";
        exit(1);
    }
}