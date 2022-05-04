#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include<vector>

#include "Type.hpp"
#include "Logger.hpp"


class Symbol {
private:
    enum CLASS { 
        CLS_VAR = 0, CLS_FUNC = 1, CLS_EXTFUNC = 2, CLS_STRUCT = 3
    };
    enum MEMORY {
        MEM_GLOBAL = 0, MEM_ARG = 1, MEM_LOCAL = 2
    };
    
    std::string class_name;
    Logger logger;
    std::string name;
    int class_;
    Type type;
    int depth;
    int memory_zone;
    std::vector<std::pair<int, Symbol>> members;

public:
    std::string get_name();
    Symbol(const std::string& name, const int& class_, const Type& type);
    Symbol();
    Symbol& operator=(const Symbol& symbol);
    Symbol create_function(const std::string& name, 
    const std::vector<Symbol>& arr,
    const int& returning_type);

    //void set_members(std::vector<std::string> members_);
    bool add_member(Symbol s);

    friend class SymbolTable;
    friend class SyntacticAnalyzer;
};

#endif