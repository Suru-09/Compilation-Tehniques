#include "VirtualMachine.hpp"

VirtualMachine::~VirtualMachine() {
    // delete stack_ptr;
    // delete stack_ptr;
}

VirtualMachine::VirtualMachine()
: stack_ptr(nullptr),
stack_after(nullptr),
n_globals(0),
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
    if ( stack_ptr - sizeof(double) < stack ) {
        std::cout << logger << "[POP_D] NOT ENOUGH BYTES ON STACK!\n";
        exit(1); 
    }
    stack_ptr -= sizeof(double);
    return *(double *)stack_ptr;
}

void VirtualMachine::push_i(const long& i) {
    if ( stack_ptr + sizeof(int) > stack_after ) {
        std::cout << logger << "[PUSH_I] OUT OF STACK!\n";
        exit(1);
    }
    *(int *)stack_ptr = i;
    stack_ptr += sizeof(long);
}

int VirtualMachine::pop_i() {
    if ( stack_ptr - sizeof(long) < stack ) {
        std::cout << logger << "[POP_I] NOT ENOUGH BYTES ON STACK!\n";
        exit(1); 
    }
    stack_ptr -= sizeof(long);
    return *(long *)stack_ptr;
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
    if ( stack_ptr - sizeof(char) < stack ) {
        std::cout << logger << "[POP_C] NOT ENOUGH BYTES ON STACK!\n";
        exit(1); 
    }
    stack_ptr -= sizeof(char);
    return *(char *)stack_ptr;
}

void VirtualMachine::push_a(void * a) {
    if ( stack_ptr + sizeof(void *) > stack_after ) {
        std::cout << logger << "[PUSH_A] OUT OF STACK!\n";
        exit(1);
    }
    *(void **)stack_ptr = a;
    stack_ptr += sizeof(void *);
}

void * VirtualMachine::pop_a() {
    if ( stack_ptr - sizeof(void *) < stack ) {
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
    char * a_val, * a_val_2;
    char * frame_ptr = 0, * old_sp;
    stack_ptr = stack;  // we allocated continuouus memory for the stack, use it
    stack_after = stack + STACK_SIZE;   // this should be the first address after
    // the stack's memory
    for ( std::list<Instruction>::iterator it = instr_list.instr_list.begin() ; it != instr_list.instr_list.end() ;) {
        // std::cout << logger << "IP: [" << (int)(*it).op_code << "] SP-stack: [" 
        //     << (stack_ptr - stack) << "]\n";
        // std::cout << logger << "STACK_PTR: [" << reinterpret_cast<void*> (&stack_ptr) << "]\n";
        switch ((*it).op_code) {
            case 0: // O_HALT
                std::cout << logger << "[O_HALT_INSTRUCTION]\n";
                return;
            case 1: // O_ADD_C
                i_val_1 = pop_c();
                i_val_2 = pop_c();
                std::cout << logger << "[O_ADD_C] Adding: " << i_val_1 << " + " << i_val_2 << " -> "
                    << "[" << i_val_1 + i_val_2 << "]\n";
                push_c(i_val_1 + i_val_2);
                ++it;
                break;
            case 2: // O_ADD_D
                d_val_1 = pop_d();
                d_val_2 = pop_d();
                std::cout << logger << "[O_ADD_D] Adding: " << d_val_1 << " + " << d_val_2 << " -> "
                    << "[" << d_val_1 + d_val_2 << "]\n";
                push_d(d_val_1 + d_val_2);
                break;
            case 3: // O_ADD_I
                i_val_1 = pop_i();
                i_val_2 = pop_i();
                std::cout << logger << "[O_ADD_I] Adding: " << i_val_1 << " + " << i_val_2 << " -> "
                    << "[" << i_val_1 + i_val_2 << "]\n";
                push_i(i_val_1 + i_val_2);
                ++it;
                break;
            case 4: // O_AND_C
                i_val_1 = pop_c();
                i_val_2 = pop_c();
                std::cout << logger << "[O_AND_C] AND: " << i_val_1 << " + " << i_val_2 << " -> "
                    << "[" << (i_val_1 && i_val_2) << "]\n";
                push_i(i_val_1 && i_val_2);
                ++it;
                break;
            case 5: // O_AND_D
                d_val_1 = pop_d();
                d_val_2 = pop_d();
                std::cout << logger << "[O_AND_D] AND: " << d_val_1 << " + " << d_val_2 << " -> "
                    << "[" << (d_val_1 && d_val_2) << "]\n";
                push_i(d_val_1 && d_val_2);
                ++it;
                break;
            case 6: // O_AND_I
                i_val_1 = pop_i();
                i_val_2 = pop_i();
                std::cout << logger << "[O_AND_I] AND: " << i_val_1 << " + " << i_val_2 << " -> "
                    << "[" << (i_val_1 && i_val_2) << "]\n";
                push_i(i_val_1 + i_val_2);
                ++it;
                break;
            case 7: // O_CALL
                std::cout << logger << "[O_CALL_INSTRUCTION]\n";
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "void") {
                    auto val = std::get<void *> ((*it).args[0]);
                    auto itr = std::find(instr_list.instr_list.begin(), instr_list.instr_list.end(), (*static_cast<Instruction *> (val)));
                    if ( itr != instr_list.instr_list.end()) {
                        push_a(reinterpret_cast<void *>(&(++it)));
                        it = itr;
                    } 
                    else {
                        std::cout << logger << "[O_CALL_INSTRUCTION] Given instruction was wrong!\n";
                        exit(3);
                    }
                }
                else {
                    std::cout << logger << "[O_CALL] Wrong structure called!\n";
                    exit(2);
                }
                break;
            case 8: // O_CALLEXT
                std::cout << logger << "[O_CALLEXT_INSTRUCTION]\n";
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "void") {
                    auto IP = std::get<void *> ((*it).args[0]);
                    (*(void(*)())IP)(); // This is really dangerous
                }
                else {
                    std::cout << logger << "[O_CALLEXT] Wrong structure called!\n";
                    exit(2);
                }
                ++it;
                break;
            case 9: // O_CAST_C_D
                i_val_1 = pop_c();
                d_val_1 = static_cast<double>(i_val_1);
                std::cout << logger << "[O_CAST_C_D] char: " << i_val_1 << "to double: " << d_val_1 << "\n";
                push_d(d_val_1);
                it++;
                break;
            case 10: // O_CAST_C_I
                i_val_1 = pop_c();
                i_val_2 = static_cast<long>(i_val_1);
                std::cout << logger << "[O_CAST_C_I] char: " << i_val_1 << "to integer: " << i_val_2 << "\n";
                push_i(i_val_2);
                it++;
                break;
            case 11:  // O_CAST_D_C
                d_val_1 = pop_d();
                i_val_1 = static_cast<char>(d_val_1);
                std::cout << logger << "[O_CAST_D_C] double: " << d_val_1 << "to char: " << i_val_1 << "\n";
                push_i(i_val_1);
                it++;   
                break;
            case 12:    // O_CAST_D_I
                d_val_1 = pop_d();
                i_val_1 = static_cast<long>(d_val_1);
                std::cout << logger << "[O_CAST_D_I] double: " << d_val_1 << "to integer: " << i_val_1 << "\n";
                push_i(i_val_1);
                it++;   
                break;
            case 13:    // O_CAST_I_C
                i_val_1 = pop_i();
                i_val_2 = static_cast<char>(i_val_1);
                std::cout << logger << "[O_CAST_I_C] integer: " << i_val_1 << "to char: " << i_val_2 << "\n";
                push_i(i_val_2);
                it++;   
                break;
            case 14:    // O_CAST_I_D
                i_val_1 = pop_i();
                d_val_1 = static_cast<double>(i_val_1);
                std::cout << logger << "[O_CAST_I_D] int: " << i_val_1 << "to double: " << d_val_1 << "\n";
                push_d(d_val_1);
                it++;
                break;
            case 15:    // O_DIV_C
                i_val_1 = pop_c();
                i_val_2 = pop_c();
                std::cout << logger << "[O_DIV_C] DIV: " << i_val_1 << " / " << i_val_2 << 
                    " -> [" << i_val_1 / i_val_2 << "]\n";
                push_c(i_val_1 / i_val_2);
                it++;
                break;
            case 16:    // O_DIV_D
                d_val_1 = pop_d();
                d_val_2 = pop_d();
                std::cout << logger << "[O_DIV_D] DIV: " << d_val_1 << " / " << d_val_2 << 
                    " -> [" << d_val_1 / d_val_2 << "]\n";
                push_d(d_val_1 / d_val_2);
                it++;
                break;
            case 17:    // O_DIV_I
                i_val_1 = pop_i();
                i_val_2 = pop_i();
                std::cout << logger << "[O_DIV_I] DIV: " << i_val_1 << " / " << i_val_2 << 
                    " -> [" << i_val_1 / i_val_2 << "]\n";
                push_i(i_val_1 / i_val_2);
                it++;
                break;
            case 18:    // O_DROP
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "long" ) {
                    i_val_1 = std::get<long> ((*it).args[0]);
                    std::cout << logger << "[O_DROP] value: " << i_val_1 << "\n";
                    if (stack_ptr - i_val_1 < stack) {
                        std::cout << logger << "[O_DROP] not enough stack bytes!\n";
                        exit(3);
                    }
                    stack_ptr -= i_val_1;
                    ++it;
                }
                else {
                    std::cout << logger << "[O_DROP] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 19:    // O_ENTER
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "long" ) {
                    i_val_1 = std::get<long> ((*it).args[0]);
                    std::cout << logger << "[O_ENTER] ENTER: " << i_val_1 << "\n";
                    push_a(frame_ptr);
                    frame_ptr = stack_ptr;
                    stack_ptr += i_val_1;
                    ++it;
                }
                else {
                    std::cout << logger << "[O_ENTER] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 20:    // O_EQ_A
                a_val = static_cast<char *> (pop_a());
                a_val_2 = static_cast<char *> (pop_a());
                std::cout << logger << "[O_EQ_A] [" << a_val << "] == [" << a_val_2 << "] -> "
                    << (d_val_1 == d_val_2) << "\n";
                push_i(d_val_1 == d_val_2);
                ++it;
                break;
            case 21:    // O_EQ_C
                i_val_1 = pop_c();
                i_val_2 = pop_c();
                std::cout << logger << "[O_EQ_C] [" << i_val_1 << "] == [" << i_val_2 << "] -> "
                    << (i_val_1 == i_val_2) << "\n";
                push_i(i_val_1 == i_val_2);
                ++it;
                break;
            case 22:    // O_EQ_D
                d_val_1 = pop_d();
                d_val_2 = pop_d();
                std::cout << logger << "[O_EQ_D] [" << d_val_1 << "] == [" << d_val_2 << "] -> "
                    << (d_val_1 == d_val_2) << "\n";
                push_i(d_val_1 == d_val_2);
                ++it;
                break;
            case 23:    // O_EQ_I
                i_val_1 = pop_i();
                i_val_2 = pop_i();
                std::cout << logger << "[O_EQ_I] [" << i_val_1 << "] == [" << i_val_2 << "] -> "
                    << (i_val_1 == i_val_2) << "\n";
                push_i(i_val_1 == i_val_2);
                ++it;
                break;
            case 24:    // O_GREATER_C
                i_val_1 = pop_c();
                i_val_2 = pop_c();
                std::cout << logger << "[O_GREATER_C] [" << i_val_1 << "] > [" << i_val_2 << "] -> "
                    << (i_val_1 > i_val_2) << "\n";
                push_i(i_val_1 > i_val_2);
                ++it;
                break;
            case 25:    // O_GREATER_D
                d_val_1 = pop_d();
                d_val_2 = pop_d();
                std::cout << logger << "[O_GREATER_D] [" << d_val_1 << "] > [" << d_val_2 << "] -> "
                    << (d_val_1 > d_val_2) << "\n";
                push_i(d_val_1 > d_val_2);
                ++it;
                break;
            case 26:    // O_GREATER_I
                i_val_1 = pop_i();
                i_val_2 = pop_i();
                std::cout << logger << "[O_GREATER_I] [" << i_val_1 << "] > [" << i_val_2 << "] -> "
                    << (i_val_1 > i_val_2) << "\n";
                push_i(i_val_1 > i_val_2);
                ++it;
                break;
            case 27:    // O_GREATEREQ_C
                i_val_1 = pop_c();
                i_val_2 = pop_c();
                std::cout << logger << "[O_GREATEREQ_C] [" << i_val_1 << "] >= [" << i_val_2 << "] -> "
                    << (i_val_1 >= i_val_2) << "\n";
                push_i(i_val_1 >= i_val_2);
                ++it;
                break;
            case 28:    // O_GREATEREQ_D
                d_val_1 = pop_d();
                d_val_2 = pop_d();
                std::cout << logger << "[O_GREATEREQ_D] [" << d_val_1 << "] >= [" << d_val_2 << "] -> "
                    << (d_val_1 >= d_val_2) << "\n";
                push_i(d_val_1 >= d_val_2);
                ++it;
                break;
            case 29:    // O_GREATEREQ_I
                i_val_1 = pop_i();
                i_val_2 = pop_i();
                std::cout << logger << "[O_GREATEREQ_I] [" << i_val_1 << "] >= [" << i_val_2 << "] -> "
                    << (i_val_1 >= i_val_2) << "\n";
                push_i(i_val_1 >= i_val_2);
                ++it;
                break;
            case 30:    // O_INSERT
                if ( (*it).args.size() == 2 && Instruction::variant_to_type((*it).args[0]) == "long"
                    && Instruction::variant_to_type((*it).args[1]) == "long" ) {
                    i_val_1 = std::get<long> ((*it).args[0]);   // Destination
                    i_val_2 = std::get<long> ((*it).args[1]);   // No. of bytes
                    std::cout << logger << "[O_INSERT]/t" << i_val_1 << ", " << i_val_2 << "\n";
                    if ( stack_ptr + i_val_2 > stack_after) {
                        std::cout << logger << "[O_INSERT] OUT of stack!\n";
                        exit(2);
                    }
                    // make room
                    memmove(stack_ptr - i_val_1 + i_val_2, stack_ptr - i_val_1, i_val_1);
                    memmove(stack_ptr - i_val_1, stack_ptr + i_val_2, i_val_2);
                    stack_ptr += i_val_2;
                    ++it;
                }
                else {
                    std::cout << logger << "[O_INSERT] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 31:    // O_JF_A
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "void" ) {
                    a_val = static_cast<char *> (pop_a());
                    auto val = std::get<void *> ((*it).args[0]);
                    std::cout << logger << "[O_JF_A] " << val << " (" << a_val << ")\n";
                    auto itr = std::find(instr_list.instr_list.begin(), instr_list.instr_list.end(), (*static_cast<Instruction *> (val)));
                    if ( itr != instr_list.instr_list.end()) {
                        it = !a_val ? itr : ++it;
                    } 
                    else {
                        std::cout << logger << "[O_JF_A] Given Instruction is not valid!\n";
                        exit(2);
                    }
                }
                else {
                    std::cout << logger << "[O_JF_A] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 32:    // O_JF_C
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "void" ) {
                    i_val_1 = pop_c();
                    auto val = std::get<void *> ((*it).args[0]);
                    std::cout << logger << "[O_JF_C] " << val << " (" << i_val_1 << ")\n";
                    auto itr = std::find(instr_list.instr_list.begin(), instr_list.instr_list.end(), (*static_cast<Instruction *> (val)));
                    if ( itr != instr_list.instr_list.end()) {
                        it = !i_val_1 ? itr : ++it;
                    } 
                    else {
                        std::cout << logger << "[O_JF_C] Given Instruction is not valid!\n";
                        exit(2);
                    }
                }
                else {
                    std::cout << logger << "[O_JF_C] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 33:    // O_JF_D
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "void" ) {
                    d_val_1 = pop_d();
                    auto val = std::get<void *> ((*it).args[0]);
                    std::cout << logger << "[O_JF_D] " << val << " (" << d_val_1 << ")\n";
                    auto itr = std::find(instr_list.instr_list.begin(), instr_list.instr_list.end(), (*static_cast<Instruction *> (val)));
                    if ( itr != instr_list.instr_list.end()) {
                        it = !d_val_1 ? itr : ++it;
                    } 
                    else {
                        std::cout << logger << "[O_JF_D] Given Instruction is not valid!\n";
                        exit(2);
                    }
                }
                else {
                    std::cout << logger << "[O_JF_D] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 34:    // O_JF_I
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "void" ) {
                    i_val_1 = pop_i();
                    auto val = std::get<void *> ((*it).args[0]);
                    std::cout << logger << "[O_JF_I] " << val << " (" << i_val_1 << ")\n";
                    auto itr = std::find(instr_list.instr_list.begin(), instr_list.instr_list.end(), (*static_cast<Instruction *> (val)));
                    if ( itr != instr_list.instr_list.end()) {
                        it = !i_val_1 ? itr : ++it;
                    } 
                    else {
                        std::cout << logger << "[O_JF_I] Given Instruction is not valid!\n";
                        exit(2);
                    }
                }
                else {
                    std::cout << logger << "[O_JF_I] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 35:    // O_JMP
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "void" ) {
                    auto val = std::get<void *> ((*it).args[0]);
                    std::cout << logger << "[O_JMP] " << val << "\n";
                    auto itr = std::find(instr_list.instr_list.begin(), instr_list.instr_list.end(), (*static_cast<Instruction *> (val)));
                    if ( itr != instr_list.instr_list.end()) {
                        it = itr;
                    } 
                    else {
                        std::cout << logger << "[O_JMP] Given Instruction is not valid!\n";
                        exit(2);
                    }
                }
                else {
                    std::cout << logger << "[O_JMP] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 36:    // O_JT_A
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "void" ) {
                    a_val = static_cast<char *> (pop_a());
                    auto val = std::get<void *> ((*it).args[0]);
                    std::cout << logger << "[O_JT_A] " << val << " (" << a_val << ")\n";
                    auto itr = std::find(instr_list.instr_list.begin(), instr_list.instr_list.end(), (*static_cast<Instruction *> (val)));
                    if ( itr != instr_list.instr_list.end()) {
                        it = a_val ? itr : ++it;
                    } 
                    else {
                        std::cout << logger << "[O_JT_A] Given Instruction is not valid!\n";
                        exit(2);
                    }
                }
                else {
                    std::cout << logger << "[O_JT_A] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 37:    // O_JT_C
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "void" ) {
                    i_val_1 = pop_c();
                    auto val = std::get<void *> ((*it).args[0]);
                    std::cout << logger << "[O_JT_C] " << val << " (" << i_val_1 << ")\n";
                    auto itr = std::find(instr_list.instr_list.begin(), instr_list.instr_list.end(), (*static_cast<Instruction *> (val)));
                    if ( itr != instr_list.instr_list.end()) {
                        it = i_val_1 ? itr : ++it;
                    } 
                    else {
                        std::cout << logger << "[O_JT_C] Given Instruction is not valid!\n";
                        exit(2);
                    }
                }
                else {
                    std::cout << logger << "[O_JT_C] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 38:    // O_JT_D
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "void" ) {
                    d_val_1 = pop_d();
                    auto val = std::get<void *> ((*it).args[0]);
                    std::cout << logger << "[O_JT_D] " << val << " (" << d_val_1 << ")\n";
                    auto itr = std::find(instr_list.instr_list.begin(), instr_list.instr_list.end(), (*static_cast<Instruction *> (val)));
                    if ( itr != instr_list.instr_list.end()) {
                        it = d_val_1 ? itr : ++it;
                    } 
                    else {
                        std::cout << logger << "[O_JT_D] Given Instruction is not valid!\n";
                        exit(2);
                    }
                }
                else {
                    std::cout << logger << "[O_JT_D] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 39:    // O_JT_I
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "void" ) {
                    i_val_1 = pop_i();
                    auto val = std::get<void *> ((*it).args[0]);
                    std::cout << logger << "[O_JT_I] " << val << " (" << i_val_1 << ")\n";
                    auto itr = std::find(instr_list.instr_list.begin(), instr_list.instr_list.end(), (*static_cast<Instruction *> (val)));
                    if ( itr != instr_list.instr_list.end()) {
                        it = i_val_1 ? itr : ++it;
                    } 
                    else {
                        std::cout << logger << "[O_JT_I] Given Instruction is not valid!\n";
                        exit(2);
                    }
                }
                else {
                    std::cout << logger << "[O_JT_I] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 40:    // O_LESS_C
                i_val_1 = pop_c();
                i_val_2 = pop_c();
                std::cout << logger << "[O_LESS_C] [" << i_val_1 << "] < [" << i_val_2 << "] -> "
                    << (i_val_1 < i_val_2) << "\n";
                push_i(i_val_1 < i_val_2);
                ++it;
                break;
            case 41:    // O_LESS_D
                d_val_1 = pop_d();
                d_val_2 = pop_d();
                std::cout << logger << "[O_LESS_D] [" << d_val_1 << "] < [" << d_val_2 << "] -> "
                    << (d_val_1 < d_val_2) << "\n";
                push_i(d_val_1 < d_val_2);
                ++it;
                break;
            case 42:    //  O_LESS_I
                i_val_1 = pop_i();
                i_val_2 = pop_i();
                std::cout << logger << "[O_LESS_I] [" << i_val_1 << "] < [" << i_val_2 << "] -> "
                    << (i_val_1 < i_val_2) << "\n";
                push_i(i_val_1 < i_val_2);
                ++it;
                break;
            case 43:    //  O_LESSEQ_C
                i_val_1 = pop_c();
                i_val_2 = pop_c();
                std::cout << logger << "[O_LESSEQ_C] [" << i_val_1 << "] <= [" << i_val_2 << "] -> "
                    << (i_val_1 <= i_val_2) << "\n";
                push_i(i_val_1 <= i_val_2);
                ++it;
                break;
            case 44:    // O_LESSEQ_D
                d_val_1 = pop_d();
                d_val_2 = pop_d();
                std::cout << logger << "[O_LESSEQ_D] [" << d_val_1 << "] <= [" << d_val_2 << "] -> "
                    << (d_val_1 <= d_val_2) << "\n";
                push_i(d_val_1 <= d_val_2);
                ++it;
                break;
            case 45:    // O_LESSEQ_I
                i_val_1 = pop_i();
                i_val_2 = pop_i();
                std::cout << logger << "[O_LESSEQ_I] [" << i_val_1 << "] <= [" << i_val_2 << "] -> "
                    << (i_val_1 <= i_val_2) << "\n";
                push_i(i_val_1 <= i_val_2);
                ++it;
                break;
            case 46:     // O_MUL_C
                i_val_1 = pop_c();
                i_val_2 = pop_c();
                std::cout << logger << "[O_MUL_C] MUL: " << i_val_1 << " * " << i_val_2 << " -> "
                    << "[" << i_val_1 * i_val_2 << "]\n";
                push_c(i_val_1 * i_val_2);
                ++it;
                break;
            case 47:    // O_MUL_D
                d_val_1 = pop_d();
                d_val_2 = pop_d();
                std::cout << logger << "[O_MUL_D] MUL: " << d_val_1 << " * " << d_val_2 << " -> "
                    << "[" << d_val_1 * d_val_2 << "]\n";
                push_c(d_val_1 * d_val_2);
                ++it;
                break;
            case 48:    // O_MUL_I
                i_val_1 = pop_i();
                i_val_2 = pop_i();
                std::cout << logger << "[O_MUL_I] MUL: " << i_val_1 << " * " << i_val_2 << " -> "
                    << "[" << i_val_1 * i_val_2 << "]\n";
                push_c(i_val_1 * i_val_2);
                ++it;
                break;
            case 49:    // O_NEG_C
                i_val_1 = pop_c();
                std::cout << logger << "[O_NEG_C] [-" << i_val_1 << "]\n";
                push_c(-i_val_1);
                ++it; 
                break;
            case 50:    // O_NEG_D
                d_val_1 = pop_d();
                std::cout << logger << "[O_NEG_D] [-" << d_val_1 << "]\n";
                push_c(-d_val_1);
                ++it; 
                break;
            case 51:    // O_NEG_I
                i_val_1 = pop_i();
                std::cout << logger << "[O_NEG_I] [-" << i_val_1 << "]\n";
                push_i(-i_val_1);
                ++it; 
                break;
            case 52:    // O_NOP
                std::cout << logger << "[O_NOP]\n";
                ++it;
                break;
            case 53:    // O_NOT_A
                a_val = static_cast<char *>(pop_a());
                std::cout << logger << "[O_NOT_A] [-" << a_val << "]\n";
                push_i(!a_val);
                ++it;
                break;
            case 54:    // O_NOT_C
                i_val_1 = pop_c();
                std::cout << logger << "[O_NOT_C] [-" << i_val_1 << "]\n";
                push_i(!i_val_1);
                ++it;
                break;
            case 55:    // O_NOT_D
                d_val_1 = pop_d();
                std::cout << logger << "[O_NOT_D] [-" << d_val_1 << "]\n";
                push_i(!d_val_1);
                ++it;
                break;
            case 56:    // O_NOT_I
                i_val_1 = pop_i();
                std::cout << logger << "[O_NOT_I] [-" << i_val_1 << "]\n";
                push_i(!i_val_1);
                ++it;
                break;
            case 57:    // O_NOTEQ_A
                a_val = static_cast<char *> (pop_a());
                a_val_2 = static_cast<char *> (pop_a());
                std::cout << logger << "[O_NOTEQ_A] [" << a_val << "] != [" << a_val_2 << "] -> "
                    << (a_val != a_val_2) << "\n";
                push_i(a_val != a_val_2);
                it++;
                break;
            case 58:    // O_NOTEQ_C
                i_val_1 = pop_c();
                i_val_2 = pop_c();
                std::cout << logger << "[O_NOTEQ_C] [" << i_val_1 << "] != [" << i_val_2 << "] -> "
                    << (i_val_1 != i_val_2) << "\n";
                push_i(i_val_1 != i_val_2);
                it++;
                break;
            case 59:    // O_NOTEQ_D
                d_val_1 = pop_d();
                d_val_2 = pop_d();
                std::cout << logger << "[O_NOTEQ_D] [" << d_val_1 << "] != [" << d_val_2 << "] -> "
                    << (d_val_1 != d_val_2) << "\n";
                push_i(d_val_1 != d_val_2);
                it++;
                break;
            case 60:    //  O_NOTEQ_I
                i_val_1 = pop_i();
                i_val_2 = pop_i();
                std::cout << logger << "[O_NOTEQ_I] [" << i_val_1 << "] != [" << i_val_2 << "] -> "
                    << (i_val_1 != i_val_2) << "\n";
                push_i(i_val_1 != i_val_2);
                it++;
                break;
            case 61:    // O_OFFSET
                i_val_1 = pop_i();
                a_val = static_cast<char *> (pop_a());
                std::cout << logger << "[O_OFFSET] " << a_val << " + " << i_val_1 << " -> "
                    << a_val + i_val_1 << "\n";
                push_a(a_val + i_val_1);
                ++it;
                break;
            case 62:    // O_OR_A
                a_val = static_cast<char *> (pop_a());
                a_val_2 = static_cast<char *> (pop_a());
                std::cout << logger << "[O_OR_A] [" << a_val << "] || [" << a_val_2 << "] -> "
                    << (a_val || a_val_2) << "\n";
                push_i(a_val || a_val_2);
                it++;
                break;
            case 63:    // O_OR_D
                d_val_1 = pop_d();
                d_val_2 = pop_d();
                std::cout << logger << "[O_OR_D] [" << d_val_1 << "] || [" << d_val_2 << "] -> "
                    << (d_val_1 || d_val_2) << "\n";
                push_i(d_val_1 || d_val_2);
                it++;
                break;
            case 64:    // O_OR_I
                i_val_1 = pop_i();
                i_val_2 = pop_i();
                std::cout << logger << "[O_OR_I] [" << i_val_1 << "] || [" << i_val_2 << "] -> "
                    << (i_val_1 || i_val_2) << "\n";
                push_i(i_val_1 || i_val_2);
                it++;
                break;
            case 65:    // O_PUSHFPADDR
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "long" ) {
                    i_val_1 = std::get<long> ((*it).args[0]);
                    std::cout << logger << "[O_PUSHFPADDR] " << i_val_1 << " " 
                        << i_val_1 + frame_ptr << "\n";
                    push_a(frame_ptr + i_val_1);
                    ++it;
                }
                else {
                    std::cout << logger << "[O_PUSHFPADDR] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 66:    // O_PUSHCT_A
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "void" ) {
                    a_val = static_cast<char * > (std::get<void*> ((*it).args[0]));
                    std::cout << logger << "[O_PUSHCT_A] " << std::hex << a_val << "\n";
                    push_a(std::get<void*> ((*it).args[0]));
                    ++it;
                }
                else {
                    std::cout << logger << "[O_PUSHCT_A] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 67:    // O_PUSHCT_C
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "long" ) {
                    i_val_1 = static_cast<char> (std::get<long> ((*it).args[0]));
                    std::cout << logger << "[O_PUSHCT_C] " << std::hex << i_val_1 << "\n";
                    push_i(std::get<long> ((*it).args[0]));
                    ++it;
                }
                else {
                    std::cout << logger << "[O_PUSHCT_C] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 68:    // O_PUSHCT_D
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "double" ) {
                    d_val_1 = std::get<double> ((*it).args[0]);
                    std::cout << logger << "[O_PUSHCT_D] " << std::hex << d_val_1 << "\n";
                    push_i(std::get<double> ((*it).args[0]));
                    ++it;
                }
                else {
                    std::cout << logger << "[O_PUSHCT_D] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 69:    // O_PUSHCT_I
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "long" ) {
                    i_val_1 = std::get<long> ((*it).args[0]);
                    std::cout << logger << "[O_PUSHCT_I] " << i_val_1 << "\n";
                    push_i(i_val_1);
                    ++it;
                }
                else {
                    std::cout << logger << "[O_PUSHCT_I] Wrong structure calling!\n";
                    exit(2);
                }
                break;
                break;
            case 70:    // O_RET
                if ( (*it).args.size() == 2 && Instruction::variant_to_type((*it).args[0]) == "long"
                    && Instruction::variant_to_type((*it).args[1]) == "long" ) {
                    i_val_1 = std::get<long> ((*it).args[0]);   // size args
                    i_val_2 = std::get<long> ((*it).args[1]);   // sizeof(retType)
                    std::cout << logger << "[O_RET] " << i_val_1 << " " << i_val_2 << "\n";
                    old_sp = stack_ptr;
                    stack_ptr = frame_ptr;
                    frame_ptr = static_cast<char *> (pop_a());
                    if ( stack_ptr - i_val_1 < stack ) {
                        std::cout << logger << "[O_RET] Not enough stack!\n";
                        exit(2);
                    }
                    stack_ptr -= i_val_1;
                    memmove(stack_ptr, old_sp - i_val_2, i_val_2);
                    stack_ptr+= i_val_2;
                }
                else {
                    std::cout << logger << "[O_RET] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 71:    // O_STORE
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "long" ){
                    i_val_1 = std::get<long> ((*it).args[0]);
                    if ( stack_ptr - (sizeof(void *) + i_val_1 ) < stack ) {
                        std::cout << logger << "[O_STORE] Not enough stack bytes for SET!\n";
                        exit(2);
                    }
                    a_val = stack_ptr - sizeof(void *) - i_val_1;
                    std::cout << logger << "[O_STORE] storing " << i_val_1 << " at "
                        << static_cast<void *> (&a_val) << "\n";
                    memcpy(a_val, stack_ptr - i_val_1, i_val_1);
                    stack_ptr -= sizeof(void *) + i_val_1;
                    // std::cout << logger << "Stack_ptr : " << static_cast<void *> (&stack_ptr)
                    //     << "  vs  " << static_cast<void *> (&stack) << "\n";
                    ++it;
                }
                else {
                    std::cout << logger << "[O_STORE] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 72:    // O_SUB_C
                i_val_1 = pop_c();
                i_val_2 = pop_c();
                std::cout << logger << "[O_SUB_C] " << i_val_1 << " - " << i_val_2 << " -> "
                    << i_val_1 - i_val_2 << "\n";
                push_d(i_val_1 - i_val_2);
                ++it;
                break;
            case 73:    // O_SUB_D
                d_val_1 = pop_d();
                d_val_2 = pop_d();
                std::cout << logger << "[O_SUB_D] " << d_val_2 << " - " << d_val_1 << " -> "
                    << d_val_2 - d_val_1 << "\n";
                push_d(d_val_2 - d_val_1);
                ++it;
                break;
            case 74:    //  O_SUB_I
                i_val_1 = pop_i();
                i_val_2 = pop_i();
                std::cout << logger << "[O_SUB_I] " << i_val_1 << " - " << i_val_2 << " -> "
                    << i_val_1 - i_val_2 << "\n";
                push_d(i_val_1 - i_val_2);
                ++it;
                break;
            case 75:    // O_LOAD
                if ( (*it).args.size() == 1 && Instruction::variant_to_type((*it).args[0]) == "long" ) {
                    i_val_1 = std::get<long> ((*it).args[0]);
                    a_val = static_cast<char*> (pop_a());
                    std::cout << logger << "[O_LOAD]\t" << i_val_1 << " at " << 
                        static_cast<void *> (&a_val) << "\n";
                    if ( stack_ptr + i_val_1 > stack_after ) {
                        std::cout << logger << "[O_LOAD] Out of stack!\n";
                        exit(2);
                    }
                    memcpy(stack_ptr, a_val, i_val_1);  // loading from address a_val [i_val_1] bytes on stack
                    stack_ptr += i_val_1;
                    it++;
                }
                else {
                    std::cout << logger << "[O_LOAD] Wrong structure calling!\n";
                    exit(2);
                }
                break;
            case 76:    // O_AND_A
                a_val = static_cast<char *> (pop_a());
                a_val_2 = static_cast<char*> (pop_a());
                std::cout << logger << "[O_AND_A] Adding: " << a_val << " + " << a_val_2 << " -> "
                    << "[" << (a_val && a_val_2) << "]\n";
                push_i(a_val && a_val_2);
                ++it;
                break;
            case 77:    // O_OR_C
                i_val_1 = pop_c();
                i_val_2 = pop_c();
                std::cout << logger << "[O_OR_C] [" << i_val_1 << "] || [" << i_val_2 << "] -> "
                    << (i_val_1 || i_val_2) << "\n";
                push_i(i_val_1 || i_val_2);
                it++;
                break;

            default:
                std::cout << logger << "[O_NONE] No instructionm with the given code!\n";
                exit(2);
        }
    }
}
