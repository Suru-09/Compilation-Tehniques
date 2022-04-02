#ifndef SYNTACTIC_ANAYLYZER_HPP
#define SYNTACTIC_ANAYLYZER_HPP

#include "LexicalAnalyzer.hpp"
#include "Logger.hpp"
#include "Utils.hpp"

class SyntacticAnalyzer {
private:
    LexicalAnalyzer lex;
    Logger logger;
    std::string class_name;
    std::shared_ptr<Node> current_token, consumed_token;

    //  STATEMENTS
    int stm_block();
    int stm();

    int expr();
    void term();
    int match(const int& code);

    //  KEYWORDS
    int r_while();
    int r_return();

    int array_decl();
    int type_base();
    int type_name();
    int decl_var();
    int decl_struct();
    int decl_func();
    int func_arg();

public:
    void unit();

    SyntacticAnalyzer(const LexicalAnalyzer &lex_analiz);
    SyntacticAnalyzer();
};

#endif