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
        std::cout << logger <<" [INSERT_AFTER] Element: [" << i << "] NOT FOUND in INSTRUCTION_LIST!\n";
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

void InstructionList::update_instr(const Instruction& to_update) {
    auto it = std::find(instr_list.begin(), instr_list.end(), to_update);
    if ( it != instr_list.end() ) {
        insert_instr_after( (*it), to_update);
        instr_list.erase(it);
    }
    else {
        std::cout << logger << "[UPDATE_INSTR] Element: [" << to_update << "] NOT FOUND in INSTRUCTION_LIST!\n";
        exit(1);
    }
}


void InstructionList::print_instruction_list() {
    std::vector<std::string> headers{"Count", "Operation CODE", "Arg 1", "Arg 2"};
    VariadicTable<int, int, std::string, std::string> vt(headers);
    vt.setColumnFormat({VariadicTableColumnFormat::AUTO,
                        VariadicTableColumnFormat::AUTO,
                        VariadicTableColumnFormat::AUTO,
                        VariadicTableColumnFormat::AUTO});

    std::cout << logger << " Printing the Instruction List: \n";
    int counter = 0;
    for (const auto& x: instr_list) {
        counter++;
        int op_code = x.op_code;
        std::string s1 = std::string{"Empty"}, s2 = std::string{"Empty"};
        int count = 0;
        for (const auto& y: x.args) {
             if ( count == 0) {
                 s1 = x.variant_to_type(y);
             }
             else if( count == 1) {
                 s2 = x.variant_to_type(y);
             }
             count++;
        }
        vt.addRow(counter, op_code, s1, s2);
    }
    vt.print(std::cout);    
}