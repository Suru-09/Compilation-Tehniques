#ifndef LEXICAL_ANALYZER_HPP
#define LEXICAL_ANALYZER_HPP

class LexicalAnalyzer {
private:
    enum TYPES {
        	ID, BREAK, CHAR, DOUBLE, ELSE, FOR, IF, INT, RETURN, STRUCT, VOID,
            WHILE, CT_INT, CT_REAL, CT_CHAR, CT_STRING, COMMA, SEMICOLON,
            LPAR, RPAR, LBRACKET, RBRACKET, LACC, RACC, END, ADD, SUB, DIV,
            MUL, DIV, DOT, AND, OR, NOT, ASSIGN, EQUAL, NOTEQ, LESS, LESSEQ,
            GREATER, GREATERQ
    };
public:
    int get_next_token();
}

#endif