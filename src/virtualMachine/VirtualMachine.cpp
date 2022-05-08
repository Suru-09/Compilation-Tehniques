#include "VirtualMachine.hpp"

VirtualMachine::~VirtualMachine() {
    delete stack_ptr;
    delete stack_ptr;
}

VirtualMachine::VirtualMachine()
: stack_ptr(nullptr),
stack_after(nullptr),
class_name("VirtualMachine")
{
    logger = Logger{class_name};
}

void VirtualMachine::push_d(const double& d) {
    if ( stack_ptr + sizeof(double) > stack_after ) {
        std::cout << logger << "[PUSH_D] OUT OF STACK!\n";
        exit(1);
    }
    *(double *)stack_ptr = d;
    stack_ptr += sizeof(double);
}

double VirtualMachine::pop_d() {
    if ( stack_ptr - sizeof(double) < stack_ptr ) {
        std::cout << logger << "[POP_D] NOT ENOUGH BYTES ON STACK!\n";
        exit(1); 
    }
    stack_ptr -= sizeof(double);
    return *(double *)stack_ptr;
}

void VirtualMachine::push_i(const int& i) {
    if ( stack_ptr + sizeof(int) > stack_after ) {
        std::cout << logger << "[PUSH_I] OUT OF STACK!\n";
        exit(1);
    }
    *(int *)stack_ptr = i;
    stack_ptr += sizeof(int);
}

int VirtualMachine::pop_i() {
    if ( stack_ptr - sizeof(int) < stack_ptr ) {
        std::cout << logger << "[POP_I] NOT ENOUGH BYTES ON STACK!\n";
        exit(1); 
    }
    stack_ptr -= sizeof(int);
    return *(int *)stack_ptr;
}

void VirtualMachine::push_c(const char& c) {
    if ( stack_ptr + sizeof(char) > stack_after ) {
        std::cout << logger << "[PUSH_C] OUT OF STACK!\n";
        exit(1);
    }
    *(char *)stack_ptr = c;
    stack_ptr += sizeof(char);
}

char VirtualMachine::pop_c() {
    if ( stack_ptr - sizeof(char) < stack_ptr ) {
        std::cout << logger << "[POP_C] NOT ENOUGH BYTES ON STACK!\n";
        exit(1); 
    }
    stack_ptr -= sizeof(char);
    return *(char *)stack_ptr;
}

void VirtualMachine::push_a(void * a) {
    if ( stack_ptr + sizeof(void *) > stack_after ) {
        std::cout << logger << "[PUSH_Aa] OUT OF STACK!\n";
        exit(1);
    }
    *(void **)stack_ptr = a;
    stack_ptr += sizeof(void *);
}

void * VirtualMachine::pop_a() {
    if ( stack_ptr - sizeof(void *) < stack_ptr ) {
        std::cout << logger << "[POP_A] NOT ENOUGH BYTES ON STACK!\n";
        exit(1); 
    }
    stack_ptr -= sizeof(void *);
    return *(void **)stack_ptr;
}

void* VirtualMachine::alloc_heap(const int& size)
{
    if ( n_globals + size > GLOBAL_SIZE ) {
        std::cout << logger << "There is not enough memory on the Heap!\n";
        exit(2);
    }
    void * p = globals + n_globals;
    n_globals += size;
    return p;
}

void VirtualMachine::set_il(const InstructionList& instr_list) {
    this->instr_list = instr_list;
}

InstructionList VirtualMachine::get_il() {
    return instr_list;
}

void VirtualMachine::run() {
    long int i_val_1, i_val_2;
    double d_val_1, d_val_2;
    char * a_val;
    char * frame_ptr = 0, old_sp;
    stack_ptr = stack;
    stack_after = stack + STACK_SIZE;
    for ( const auto& it: instr_list.instr_list) {
        std::cout << logger << " IP: " << it << " SP-stack: " << stack_ptr - stack << "\n";
        switch (it.op_code) {
            case 0: // O_HALT
                return;
        }
    }
}
