#ifndef VIRTUAL_MACHINE_HPP
#define VIRTUAL_MACHINE_HPP

#include <stack>
#include <any>

#include "Logger.hpp"

#define STACK_SIZE (32 * 1024)

class VirtualMachine {
private:
    char stack[STACK_SIZE];
    char *stack_ptr, *stack_after;

    std::string class_name;
    Logger logger;
public:
    VirtualMachine();
    ~VirtualMachine();

    // STACK METHODS
    void push_d(const double& d);
    double pop_d();
    void push_i(const int& i);
    int pop_i();
    void push_c(const char& c);
    char pop_c();
    void push_a(void* a);
    void * pop_a();
};

#endif