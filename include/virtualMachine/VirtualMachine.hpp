#ifndef VIRTUAL_MACHINE_HPP
#define VIRTUAL_MACHINE_HPP

#include <string.h>
#include<iostream>
#include <map>
#include <variant>
#include <memory>


#include "Logger.hpp"
#include "Instruction.hpp"
#include "InstructionList.hpp"
#include "Symbol.hpp"
#include "Type.hpp"

#define STACK_SIZE (1024 * 32)
#define GLOBAL_SIZE (1024 * 1024)

class VirtualMachine {
private:
    char stack[STACK_SIZE] = {0}, globals[GLOBAL_SIZE] = {0};
    char *stack_ptr, *stack_after;
    int n_globals;
    InstructionList instr_list;
    std::map<std::pair<std::string, long>, 
        std::variant<long, double, void *>> registers;


    std::string class_name;
    Logger logger;
public:
    VirtualMachine();
    ~VirtualMachine();

    void set_il(const InstructionList& instr_list);
    InstructionList get_il();

    // alloc global
    void* alloc_heap(const int& size);

    // STACK METHODS
    void push_d(const double& d);
    double pop_d();
    void push_i(const long& i);
    int pop_i();
    void push_c(const char& c);
    char pop_c();
    void push_a(void* a);
    void * pop_a();
    void push_in_register(const Symbol& symb,
        const std::variant<long, double, void*>& symb_val);
    std::variant<long, double, void *> search_register(const Symbol& symb);
    void clear_register_level(const long& depth);

    // RUN METHODS
    void run();

    friend class SyntacticAnalyzer;
};

#endif