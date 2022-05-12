#ifndef SYNTACTIC_ANAYLYZER_HPP
#define SYNTACTIC_ANAYLYZER_HPP

#include "LexicalAnalyzer.hpp"
#include "Logger.hpp"
#include "Utils.hpp"
#include "SymbolTable.hpp"
#include "Symbol.hpp"
#include "Type.hpp"
#include "ReturnValue.hpp"
#include "VirtualMachine.hpp"

class SyntacticAnalyzer {
private:
    LexicalAnalyzer lex;
    Logger logger;
    std::string class_name;
    std::shared_ptr<Node> current_token, consumed_token;
    //SymbolTable
    SymbolTable symbol_table;
    int current_depth;
    std::string current_struct;
    std::string current_func;
    Symbol tmp;
    bool is_struct;
    //Type checking
    ReturnValue ret_val;
    // Virtual Machine
    inline static VirtualMachine vm = VirtualMachine{};

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
    bool add_args_symbol_table(Type& type);

    // TYPE CHECKING
    void cast_type(const Type& src, const Type& dest);
    Type get_arithmetic_type(const Type& type1, const Type& type2);
    Type create_type(const int& type_base, const int& elements);
    Symbol tc_search_function();
    void check_postfix(const ReturnValue& rv);
    void check_unary_sub();
    void check_unary_not();
    void check_mul(const ReturnValue& rv);
    void check_add(const ReturnValue& rv);
    void check_rel(const ReturnValue& rv);
    void check_eq(const ReturnValue& rv);
    void check_or(const ReturnValue& rv);
    void check_and(const ReturnValue& rv);
    void check_assign(const ReturnValue& rv);
    void check_r_if();
    void check_r_return();
    void check_array_decl();
    void check_cast(const Type& type);
    Symbol find_val_in_members(const Symbol& symb, const std::string& symb_name);

    // VM STUFF
    void add_predefined_functions();
    void add_ext_func(const std::string &str, 
        std::vector<Symbol> arr, 
        const int& type);
    void static put_s();
    void static get_s();
    void static put_i();
    void static get_i();
    void static put_d();
    void static get_d();
    void static put_c();
    void static get_c();
    void static seconds();

    //CODE GENERATION
    long type_base_size(const Type& type);

public:
    void unit();
    void test_vm();

    SyntacticAnalyzer(const LexicalAnalyzer &lex_analiz);
    SyntacticAnalyzer();
};

#endif