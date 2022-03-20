#ifndef TOKEN_HPP
#define TOKEN_HPP

#include<iostream>
#include<vector>
#include <variant>

class Token {

private:
        int code;
        std::variant<std::vector<char>, long int, double> text;
        int line;
public:
    Token(const int& code, const long int& value_int, const int& line)
    : code(code),
    text(value_int),
    line(line) {};

    Token(const int& code, const std::vector<char>& text, const int& line)
    : code(code),
    text(text),
    line(line) {};

    Token(const int& code, const double& value_double, const int& line)
    : code(code),
    text(value_double),
    line(line) {};

    explicit Token() = default;
};

#endif