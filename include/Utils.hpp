#ifndef UTILS_HPP
#define UTILS_HPP

#include<iostream>
#include<fstream>
#include<string>
#include <vector>
#include<stdlib.h>
#include "Symbol.hpp"
#include "SymbolTable.hpp"

namespace utils {

    std::vector<char> read_file(const std::string& file_name);
    void test_file(const std::string& file_name);
    void test_diagrams();
    void test_symbol_structure();
    std::string class_to_string(int class_);
    std::string type_to_string(int type);
    std::string log_error(const int& line, const std::string& message);
    void add_predefined_functions(SymbolTable& symbol_table);
}

#endif