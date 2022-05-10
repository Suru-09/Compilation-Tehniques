#ifndef VIRTUAL_MACHINE_HPP
#define VIRTUAL_MACHINE_HPP

#include <string.h>

#include "Logger.hpp"
#include "Instruction.hpp"
#include "InstructionList.hpp"

#define STACK_SIZE (32 * 1024)
#define GLOBAL_SIZE (32 * 1024)

class VirtualMachine {
private:
    char stack[STACK_SIZE], globals[GLOBAL_SIZE];
    char *stack_ptr, *stack_after;
    int n_globals;
    InstructionList instr_list;


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

    // RUN METHODS
    void run();

    friend class SyntacticAnalyzer;
};

#endif