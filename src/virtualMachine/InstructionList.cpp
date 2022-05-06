#include "InstructionList.hpp"

InstructionList::InstructionList()
: class_name("InstructionList"),
logger(Logger{class_name})
{}

void InstructionList::insert_instr_after(const Instruction& after, const Instruction& i) {
    auto it = std::find(instr_list.begin(), instr_list.end(), after);
    if( it != instr_list.end() ) {
        instr_list.insert(it, i);
    }
    else {
        std::cout << logger << "Element: [" << i << "] NOT FOUND in INSTRUCTION_LIST!\n";
        exit(1);
    }
}

void InstructionList::insert_instr(const Instruction& i) {
    instr_list.push_back(i);
}

void InstructionList::create_insert_instr_after(const Instruction& after, const int& op_code) {
    insert_instr_after(after, Instruction{op_code});
}

void InstructionList::create_insert_instr(const int& op_code) {
    insert_instr(Instruction{op_code});    
}

void InstructionList::delete_instr_after(const Instruction& start) {
    auto it = std::find(instr_list.begin(), instr_list.end(), start);
    if( it != instr_list.end() ) {
        instr_list.erase(it, instr_list.end());
    }
    else {
        std::cout << logger << "Element: [" << start << "] NOT FOUND in INSTRUCTION_LIST!\n";
        exit(1);
    }    
}