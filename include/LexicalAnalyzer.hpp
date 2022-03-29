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
            GREATER = 26, DIV = 27,  CT_INT = 36, CT_REAL = 44, EQUAL = 46, ASSIGN = 47, CT_CHAR = 51,
            CT_STRING = 55, ID = 57, END = -1,
            BREAK = 60, CHAR = 61, DOUBLE = 62, ELSE = 63, FOR = 64, INT = 65, IF = 66,
            RETURN = 67, STRUCT = 68, VOID = 69, WHILE = 70
    };

    std::map<TYPES, std::string> pretty_map;

    TokenList token_list;
    Logger logger;
    std::vector<char> given_text;
    long unsigned int i;
    int line;
    std::string class_name;

    void add_token(const int& code, const std::string& text, const int& line);
    void add_token(const int& code, const long int& text, const int& line);
    void add_token(const int& code, const double& text, const int& line);
    void add_token(const int& code, const int& line);
    void init_map();
    std::string return_keyword(const std::string &str);
    
public:
    LexicalAnalyzer(const std::vector<char>& text);
    LexicalAnalyzer();

    int get_next_token();
    void get_all_tokens();
    std::string extract(int start, const int& end);
    std::string print_pretty(int id);
};

#endif