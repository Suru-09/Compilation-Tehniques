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
            GREATER = 26, CT_INT = 33, CT_REAL = 44, CT_CHAR, CT_STRING, ID = 52
    };

    TokenList token_list;
    std::vector<char> given_text;
    int i;
    int line;

public:
    LexicalAnalyzer(const std::vector<char>& text);

    int get_next_token();
    void get_all_tokens();
    std::vector<char> extract(int start, const int& end);
};

#endif