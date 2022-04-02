#ifndef UTILS_HPP
#define UTILS_HPP

#include<iostream>
#include<fstream>
#include<string>
#include <vector>
#include<stdlib.h>

// #include "Token.hpp"
// #include "TokenList.hpp"
// #include "Node.hpp"

namespace utils {


    std::vector<char> read_file(const std::string& file_name);
    void test_file(const std::string& file_name);
    void test_diagrams();
    std::string log_error(const int& line, const std::string& message);
}

#endif