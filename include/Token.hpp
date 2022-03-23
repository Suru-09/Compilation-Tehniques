#ifndef TOKEN_HPP
#define TOKEN_HPP

#include<iostream>
#include<vector>
#include <variant>

class Token {

public:
    int code;
    std::variant<std::string, long int, double> text;
    int line;

    Token(const int& code, const long int& value_int, const int& line);
    Token(const int& code, const std::string& text, const int& line);
    Token(const int& code, const double& value_double, const int& line);
    Token& operator=(const Token& token);
    Token(const int& code);

    explicit Token() = default;
};

#endif