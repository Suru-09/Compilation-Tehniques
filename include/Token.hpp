#ifndef TOKEN_HPP
#define TOKEN_HPP

#include<iostream>
#include<vector>
#include <variant>

#include "Logger.hpp"

class Token {
public:
    Logger logger;
    int code;
    std::variant<std::string, long int, double> text;
    int line;

    Token(const int& code, const long int& value_int, const int& line);
    Token(const int& code, const std::string& text, const int& line);
    Token(const int& code, const int& line);
    Token(const int& code, const double& value_double, const int& line);
    Token& operator=(const Token& token);
    Token(const int& code);

    void print_text();

    explicit Token() = default;
private:
    std::string class_name;
};

#endif