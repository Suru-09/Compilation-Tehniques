#ifndef TYPE_HPP
#define TYPE_HPP

#include<iostream>

class Type {
private:
    int type_base;
    std::string symbol_name;     // Only for TB_STRUCT
    int elements;   // <0 NON-ARRAY == 0 Array without size and > 0 array's size
public:
    enum TypeBase {
        TB_INT = 0, TB_DOUBLE = 1, TB_CHAR = 2, TB_STRING = 3, TB_STRUCT = 4, 
        TB_VOID = 5
    };

    explicit Type() = default;
    Type(const int& type_base, const std::string& symbol_name = "", const int& elements = -1);
    Type& operator=(const Type& type);

    friend class Symbol;
    friend class SymbolTable;
    friend class SyntacticAnalyzer;
};

#endif