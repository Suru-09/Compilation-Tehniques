#ifndef LEXICAL_ANALYZER_HPP
#define LEXICAL_ANALYZER_HPP

#include <iostream>
#include<vector>
#include <map>
#include "TokenList.hpp"

class LexicalAnalyzer {
private:
    enum TYPES {
        	COMMA = 1, SEMICOLON = 2, LPAR = 3, RPAR = 4, LBRACKET = 5, RBRACKET = 6,
            LACC = 7, RACC = 8, ADD = 9, SUB = 10, MUL = 11, DOT = 13, AND = 15, OR = 17,
            NOTEQ = 19, NOT = 20, LESSEQ = 22, LESS = 23, GREATEREQ = 25,
            GREATER = 26, CT_INT = 36, CT_REAL = 44, CT_CHAR, CT_STRING, ID = 52, END = -1,
            BREAK = 60, CHAR = 61, DOUBLE = 62, ELSE = 63, FOR = 64, INT = 65, IF = 66,
            RETURN = 67, STRUCT = 68, VOID = 69, WHILE = 70
    };

    TokenList token_list;
    std::vector<char> given_text;
    int i;
    int line;

    void add_token(const int& code, const std::string& text, const int& line);
    void add_token(const int& code, const long int& text, const int& line);
    void add_token(const int& code, const double& text, const int& line);

public:
    LexicalAnalyzer(const std::vector<char>& text);

    int get_next_token();
    void get_all_tokens();
    std::string extract(int start, const int& end);
};

#endif