#include "Token.hpp"

Token::Token(const int& code, const long int& value_int, const int& line)
: code(code),
text(value_int),
line(line),
class_name("Token")
{
    logger = Logger{class_name};
}

Token::Token(const int& code, const std::string& text, const int& line)
: code(code),
text(text),
line(line),
class_name("Token")
{
    logger = Logger{class_name};
}

Token::Token(const int& code, const double& value_double, const int& line)
: code(code),
text(value_double),
line(line),
class_name("Token")
{
    logger = Logger{class_name};
}

Token::Token(const int& code, const int& line)
: code(code),
line(line),
class_name("Token")
{
    logger = Logger{class_name};
}

Token& Token::operator=(const Token& token) {
    code = token.code;
    text = token.text;
    line = token.line;

    return *this;
}

void Token::print_text() {
    try {
        std::string text_2 = std::get<std::string> (text);
        if(text_2 == "")
            text_2 = "---";
        std::cout << logger << " " << text_2 << "\n";
        }
    catch(...) {
        try {
            long int integer = std::get<long int> (text);
            std::cout << logger << " " << integer << "\n";
        }
        catch(...) {
            try {
                double dbl = std::get<double> (text);
                std::cout << logger << " " << dbl << "\n";
            }
            catch(...) {
                // std::cout << logger << "Error while printing the list of tokens!\n";
            }
        }
    }
}

Token::Token(const int& code)
: code(code),
class_name("Token")
{
    logger = Logger{class_name};
}