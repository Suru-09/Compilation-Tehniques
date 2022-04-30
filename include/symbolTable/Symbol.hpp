#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <variant>
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
    /**
     * @brief Can be members for structs or parameters
     for functions
     */
    std::pair<CLASS, std::vector<std::string>> members;

public:
    std::string get_name();
    Symbol(const std::string& name, const int& class_, const Type& type);
    Symbol();
    Symbol& operator=(const Symbol& symbol);

    void set_members(std::vector<std::string> members_);

    friend class SymbolTable;
    friend class SyntacticAnalyzer;
};

#endif