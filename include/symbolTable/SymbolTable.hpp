#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <bits/stdc++.h>
#include "Symbol.hpp"
#include "Logger.hpp"


class SymbolTable {
private:
    std::string class_name;
    Logger logger;
    std::multimap<std::string, Symbol> symbol_table;
public:
    SymbolTable();
    void add_symbol(Symbol& symbol);
    Symbol find_symbol(const std::string& key);
    void delete_symbol(const std::string& key);
    void print_symbol_table();

    friend class SyntacticAnalyzer;
};

#endif