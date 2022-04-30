#ifndef SYNTACTIC_ANAYLYZER_HPP
#define SYNTACTIC_ANAYLYZER_HPP

#include "LexicalAnalyzer.hpp"
#include "Logger.hpp"
#include "Utils.hpp"
#include "SymbolTable.hpp"
#include "Symbol.hpp"
#include "Type.hpp"

class SyntacticAnalyzer {
private:
    LexicalAnalyzer lex;
    Logger logger;
    std::string class_name;
    std::shared_ptr<Node> current_token, consumed_token;
    SymbolTable symbol_table;
    int current_depth;
    std::string current_struct;
    std::string current_func;
    Symbol tmp;

    //  STATEMENTS
    int stm_block();
    int stm();

    // EXPRESSIONS
    int expr();
    int expr_assign_helper();
    int expr_assign();
    int expr_or_helper();
    int expr_or();
    int expr_and_helper();
    int expr_and();
    int expr_eq_helper();
    int expr_eq();
    int expr_rel_helper();
    int expr_rel();
    int expr_add();
    int expr_add_helper();
    int expr_mul();
    int expr_mul_helper();
    int expr_cast();
    int expr_unary();
    int expr_postfix();
    int expr_postfix_bracket();
    int expr_primary();
    
    // MATCH A TOKEN
    int match(const int& code);

    //  KEYWORDS
    int r_while();
    int r_return();
    int r_if();
    int r_for();
    int r_break();
    int r_optional_expr();

    int array_decl();
    int type_base();
    int type_name();
    int decl_var();
    int decl_struct();
    int decl_func();
    int func_arg();

    // SEMANTIC ANALYSIS
    bool add_var(Type type, std::string name);
    Symbol add_decl_var_to_symbol(Type& type);
    Type type_base_condition();
    void check_if_struct_exists();
    void check_struct_helper();
    Symbol check_decl_func_helper(Type& type);

public:
    void unit();

    SyntacticAnalyzer(const LexicalAnalyzer &lex_analiz);
    SyntacticAnalyzer();
};

#endif