#include "Token.hpp"

Token::Token(const int& code, const long int& value_int, const int& line)
: code(code),
text(value_int),
line(line) 
{}

Token::Token(const int& code, const std::string& text, const int& line)
: code(code),
text(text),
line(line) 
{}

Token::Token(const int& code, const double& value_double, const int& line)
: code(code),
text(value_double),
line(line) 
{}

Token::Token(const int& code, const int& line)
: code(code),
line(line)
{}

Token& Token::operator=(const Token& token) {
    code = token.code;
    text = token.text;
    line = token.line;

    return *this;
}

Token::Token(const int& code)
: code(code) 
{}